#!/usr/bin/env python3
import subprocess

# feed a simple sequence via a pipe inside gdb
cmds = """
set pagination off
set confirm off
file ./babyallocator
break *0x401350
commands
silent
printf "GUARDCHECK rsp=%p rax(size1)=%p guard=%p\\n", $rsp, $rax, *(unsigned long*)($fs_base+0x70)
continue
end
break *0x401369
commands
silent
printf "  -> STACK path: rsp=%p\\n", $rsp
continue
end
break *0x401362
commands
silent
printf "  -> MORESTACK path: size=%p rsp=%p\\n", $rdi, $rsp
continue
end
break *0x4017d0
commands
silent
printf "__generic_morestack frame=%p param=%p\\n", *(unsigned long*)$rdi, $rdx
continue
end
break *0x401e30
commands
silent
printf "allocate_stack_space size=%p\\n", $rdi
continue
end
run < input.txt
quit
"""
open('gdbcmds.txt','w').write(cmds)

# input: case1 size 0x200 -> write -> case4 -> case1 size 0x200 -> write -> 5 -> 5 -> 6
inp  = b"1\n512\nname1\n3\n" + b"C"*512 + b"\n"
inp += b"4\n1\n512\nname2\n3\n" + b"D"*512 + b"\n5\n5\n6\n"
open('input.txt','wb').write(inp)

subprocess.run(['gdb','-q','-batch','-x','gdbcmds.txt'], timeout=120)
