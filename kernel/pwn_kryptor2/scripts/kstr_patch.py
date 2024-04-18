"""
patch and convert all kernel strings starting with 0x01 byte with 0xa so ida can
interpret as str.
"""

READ  = (1<<1)
WRITE = (1<<2)
EXEC  = (1<<3)
is_ro = lambda attr : \
  (attr & READ) and not (attr & (WRITE | EXEC))
is_strsegm = lambda name : "str" in name

segments = [
  ida_segment.get_segm_by_name((get_segm_name(ea)))
    for ea in Segments() if
      is_ro() and 
]
print(segments)
