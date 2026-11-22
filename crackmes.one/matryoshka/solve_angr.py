"""Execute the original five native VM interpreters with angr.

Run with ~/.virtualenvs/angr/bin/python solve_angr.py.
No decoded bytecode, VM reimplementation, or password-derived constraints.
"""
import logging
from pathlib import Path
import time

import angr
from angr import claripy

logging.getLogger("angr").setLevel(logging.ERROR)
logging.getLogger("cle").setLevel(logging.ERROR)
BASE = 0x140000000
project = angr.Project(str(Path(__file__).with_name("crackme.exe")), auto_load_libs=False)
# Enter immediately after line input, before the first bytecode decryption.
# The original main stack frame remains allocated throughout validation.
state = project.factory.blank_state(
    addr=BASE + 0x8D58,
    add_options={
        angr.options.ZERO_FILL_UNCONSTRAINED_MEMORY,
        angr.options.ZERO_FILL_UNCONSTRAINED_REGISTERS,
    },
)
state.regs.rsp = 0x7FFF0000
state.regs.r12 = BASE + 0xC220
state.regs.r15 = BASE + 0xC0C0
length = claripy.BVS("input_length", 32)
state.regs.ebp = length
state.solver.add(claripy.UGE(length, 1), claripy.ULE(length, 511))
password = [claripy.BVS(f"input_{i}", 8) for i in range(511)]
for i, byte in enumerate(password):
    # Model the line reader: non-NUL bytes excluding CR/LF.
    state.solver.add(byte != 0, byte != 10, byte != 13)
    state.memory.store(0x7FFF0000 + 0x120 + i, byte)

simgr = project.factory.simgr(state, veritesting=True)
start = time.monotonic()
steps = 0
def progress(manager):
    global steps
    steps += 1
    if steps % 100 == 0:
        print(f"{time.monotonic()-start:.1f}s: active={len(manager.active)} "
              f"deadended={len(manager.deadended)}", flush=True)
    return manager
# Success is the taken branch after the final success-message checksum.
simgr.explore(find=BASE + 0x9A42,
              avoid=[BASE + 0x8CDE, BASE + 0x8E8E],
              step_func=progress)
if not simgr.found:
    print(simgr)
    for error in simgr.errored:
        print(error)
    raise SystemExit("No accepting state found")
found = simgr.found[0]
n = found.solver.eval(length)
answer = found.solver.eval(claripy.Concat(*password[:n]), cast_to=bytes)
Path(__file__).with_name("password_angr.txt").write_bytes(answer + b"\n")
print(f"Length: {n}\nPassword: {answer.decode()}\nElapsed: {time.monotonic()-start:.1f}s")
