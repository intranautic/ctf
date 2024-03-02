bits 16
org 0x7c00

section .text
global boot1_entry
boot1_entry:
  jmp boot1_main
  nop
  ; boot parameter block structure
  .boot_pb:
    .oem_id:        db "        "
    .sector_size:   dw 0x200
    .cluster_size:  db 0x1
    .reserved_sect: dw 0x1
    .fat_no:        db 0x2
    .root_size:     dw 0xe0
    .sector_no:     dw 0xb40
    .media_type:    db 0xf0
    .fat_size:      dw 0x9
    .track_sect:    dw 0x9
    .head_no:       dw 0x2
    .hidden_sect:   dd 0
    .bigsect_no:    dd 0
    .boot_drive:    db 0
    .reserved:      db 0
    .boot_sign:     db 0x29
    .volume_id:     dd 0
    .volume_label:  db "           "
    .fs_type:       dq 0

boot1_print:
  mov ah, 14
  mov bx, 9
  .print_loop:
    lodsb
    test al, al
    jz .print_done
    int 0x10
    jmp .print_loop
  .print_done:
    ret

boot1_main:
  ; disable int, zero out segment selector registers and set stack
  ; pointer to boot sector loaded into memory by BIOS.
  .init_segment:
    cli
    cld
    xor si, si
    mov ds, si
    mov es, si
    mov ss, si
    mov sp, 0x7c00
    sti





    jmp ($-$$)


; message strings
msg_error:    db "Boot stage_1: Error occured, rebooting!", 0xa, 0xd, 0

; padding and magic for boot sector
boot1_pad:    times 510-($-$$) db 0
boot1_magic:  dd 0xaa55
