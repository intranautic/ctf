set pagination off
b main
run
printf "FS_BASE=%#lx\n", $fs_base
printf "CANARY=%#lx\n", *(unsigned long*)($fs_base+0x28)
info auxv
kill
