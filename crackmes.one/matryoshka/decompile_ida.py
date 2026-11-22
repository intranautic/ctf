import ida_hexrays, ida_funcs
for ea in [0x140008c1c]:
    f = ida_funcs.get_func(ea)
    print(hex(f.start_ea), ida_hexrays.decompile(f.start_ea))
