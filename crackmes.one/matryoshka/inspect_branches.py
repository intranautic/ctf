import idautils, idc
for ea in idautils.Heads(0x140008b80,0x14000b000):
    text=idc.generate_disasm_line(ea,0) or ""
    if "66h" in text or "88h" in text or "0FFh" in text:
        print(hex(ea), text)
