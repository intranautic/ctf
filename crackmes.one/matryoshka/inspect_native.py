import idautils, idc
for ea in idautils.Heads(0x140008b80,0x140008f20):
    print(hex(ea),idc.generate_disasm_line(ea,0))
