#!/usr/bin/env python3
import struct
band = open('scanband.bin','rb').read()
assert len(band) == 0x1400, len(band)
low = None
for i in range(len(band)):
    if band[i] != 0xAA:
        low = i; break
print('LOWEST MODIFIED depth = 0x%x' % (0x1400 - low))
print('=== ALL written slots (depth, value) ===')
for off in range(low & ~7, 0x1400, 8):
    v = struct.unpack('<Q', band[off:off+8])[0]
    if all(b == 0xAA for b in band[off:off+8]): continue
    depth = 0x1400 - off
    print('  depth 0x%04x = 0x%012x%s' % (depth, v, '  <== ptr' if 0x7f0000000000 < v < 0x800000000000 else ''))
