#!/usr/bin/env python3
import argparse
import struct
import sys
from pathlib import Path

"""Recover the 16-byte input for Towel's KataVM Level 1. Requires Python 3.8+."""

# In this ELF, RVA 0x3008 maps to file offset 0x3008.
IMAGE_OFFSET = 0x3008
IMAGE_SIZE = 0x3B1A
MASK = 0xFFFFFFFF
SHL, SHR, ADD, CHECK_EQ = 0x7C, 0xB1, 0x4A, 0xC3

# Total encoded length for each descriptor mode, including padding.
# Zero means the native handler does not implement that mode.
LENGTHS = {
    0x1E: (3, 8, 9, 10),  # SWAP_BANKS
    0x3E: (4, 0, 7, 0),   # OR
    ADD:  (2, 0, 7, 0),
    0x5D: (3, 0, 8, 0),   # READ
    SHL:  (4, 0, 7, 0),
    0x8B: (3, 0, 6, 0),   # XOR
    0xAA: (3, 0, 8, 0),   # SUB
    SHR:  (2, 0, 6, 0),
    CHECK_EQ: (4, 0, 7, 0),
    0xD5: (4, 3, 5, 9),   # ROR_REGS
    0xEF: (2, 4, 3, 5),   # WRITE8
    0xF6: (5, 0, 6, 0),   # MOV
}


def instructions(image):
    pc = 0
    while pc + 1 < len(image):
        opcode, descriptor = image[pc:pc + 2]
        if opcode == 0:
            return
        mode = descriptor & 3
        size = LENGTHS.get(opcode, (0, 0, 0, 0))[mode]
        if not size or pc + size > len(image):
            raise ValueError(f"Invalid instruction at VM offset {pc:#x}")
        yield pc, opcode, mode
        pc += size
    raise ValueError("Missing VM terminator")


def mix(word, total, key_a, key_b, shifts):
    left, right = shifts
    return (((word << left) + key_a) ^ (word + total) ^ ((word >> right) + key_b)) & MASK


def decrypt_block(x, y, key, delta, shifts):
    # Each half-round has one SHL and one SHR. Their instruction order can vary.
    if len(shifts) != 128:
        raise ValueError("Expected four shift instructions per round for 32 rounds")
    halves = []
    for i in range(0, len(shifts), 2):
        counts = dict(shifts[i:i + 2])
        if counts.keys() != {SHL, SHR}:
            raise ValueError("Unrecognized half-round")
        halves.append((counts[SHL], counts[SHR]))

    for i in reversed(range(32)):
        total = ((i + 1) * delta) & MASK
        # The forward y update uses the updated x, so undo y first.
        y = (y - mix(x, total, key[2], key[3], halves[2*i + 1])) & MASK
        x = (x - mix(y, total, key[0], key[1], halves[2*i])) & MASK
    return struct.pack("<II", x, y)


def solve(binary):
    image = binary[IMAGE_OFFSET:IMAGE_OFFSET + IMAGE_SIZE]
    if binary[:6] != b"\x7fELF\x02\x01" or len(image) != IMAGE_SIZE:
        raise ValueError("Expected the 64-bit little-endian KataVM_L1 ELF")

    # These five ADD-immediate sites are in the first unrolled encryption round.
    def add_immediate(pc):
        if image[pc] != ADD or image[pc + 1] & 3 != 2:
            raise ValueError(f"Expected ADD-immediate at VM offset {pc:#x}")
        return struct.unpack_from("<I", image, pc + 2)[0]

    key = [add_immediate(pc) for pc in (0x06D4, 0x06DB, 0x0722, 0x072F)]
    delta = add_immediate(0x06F3)
    plaintext = bytearray()
    shifts, targets = [], []
    for pc, opcode, mode in instructions(image):
        if opcode in (SHL, SHR, CHECK_EQ) and mode != 2:
            raise ValueError(f"Expected an immediate operand at VM offset {pc:#x}")
        if opcode in (SHL, SHR):
            shifts.append((opcode, image[pc + 2] & 31))
        elif opcode == CHECK_EQ:
            targets.append(struct.unpack_from("<I", image, pc + 2)[0])
            if len(targets) == 2:
                plaintext.extend(decrypt_block(*targets, key, delta, shifts))
                shifts.clear()
                targets.clear()
    if len(plaintext) != 16 or shifts or targets:
        raise ValueError("Expected two complete checked blocks")
    return plaintext


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("binary", nargs="?", type=Path, default=Path("KataVM_L1"),
                        help="path to the challenge ELF (default: ./KataVM_L1)")
    args = parser.parse_args()
    try:
        sys.stdout.buffer.write(solve(args.binary.read_bytes()))
    except (OSError, ValueError) as error:
        parser.exit(1, f"{parser.prog}: {error}\n")
