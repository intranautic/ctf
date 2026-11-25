import idautils, ida_hexrays, ida_funcs, ida_name, ida_bytes
print("STRINGS")
for s in idautils.Strings():
    if any(w in str(s).lower() for w in ["password", "correct", "wrong", "success", "fail"]):
        print(hex(s.ea), str(s), [hex(x.frm) for x in idautils.XrefsTo(s.ea)])
