import ida_bytes, ida_hexrays, json
from pathlib import Path
root = Path(r"\\wsl.localhost\Ubuntu\home\folly\ctf\crackmes.one\matryoshka")
data = {hex(ea): ida_bytes.get_bytes(ea, size).hex() for ea,size in [
    (0x14000c1c0,71),(0x14000c207,4),(0x14000c220,256),
    (0x14000c320,269),(0x14000c440,1172),(0x14000c8e0,617),
    (0x14000cb60,974),(0x14000cf40,1720),(0x14000d600,272)]}
(root/"vm_data.json").write_text(json.dumps(data,indent=2))
(root/"main_decompiled.c").write_text(str(ida_hexrays.decompile(0x140008b80)))
print("Saved VM blobs and main pseudocode.")
