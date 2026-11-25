"""Run inside IDA through ida_mcp.execute_python, after opening crackme.exe."""
import json
from pathlib import Path
import ida_bytes
import ida_frame
import ida_funcs
import ida_hexrays
import ida_name
import ida_nalt
import ida_typeinf
import ida_xref
import idautils
import idc

ROOT = Path(r"\\wsl.localhost\Ubuntu\home\folly\ctf\crackmes.one\matryoshka")
MAIN = 0x140008B80
changes = []

def name(ea, new):
    old = ida_name.get_name(ea)
    if old != new:
        if not ida_name.set_name(ea, new, ida_name.SN_CHECK):
            raise RuntimeError(f"Cannot name {ea:x} {new}")
        changes.append({"address": hex(ea), "old_name": old, "name": new})

def comment(ea, text):
    old = ida_bytes.get_cmt(ea, False)
    if old and text in old:
        return
    if old:
        text = old + "\n" + text
    ida_bytes.set_cmt(ea, text, False)

def signature(ea, decl):
    if not idc.SetType(ea, decl):
        raise RuntimeError(f"Cannot apply type at {ea:x}: {decl}")

decls = r"""
typedef unsigned char vm_u8;
typedef unsigned int vm_u32;
typedef struct VmRegisterFile32 { vm_u32 r[8]; } VmRegisterFile32;
typedef struct Vm3StackAndInput {
    vm_u32 stack[32];
    char password[512];
} Vm3StackAndInput;
typedef union Vm4CodeOrMessage {
    vm_u8 bytecode[1172];
    char success_message[72];
    vm_u8 allocation[4096];
} Vm4CodeOrMessage;
typedef struct MatryoshkaWorkspace {
    VmRegisterFile32 vm1;
    VmRegisterFile32 vm2;
    VmRegisterFile32 vm4;
    vm_u32 vm3_stack[32];
    vm_u8 password[512];
    vm_u8 vm5_code[1024];
    vm_u8 vm1_code[4096];
    vm_u8 vm2_code[4096];
    vm_u8 vm3_code[4096];
    Vm4CodeOrMessage vm4_or_message;
} MatryoshkaWorkspace;
typedef enum Vm1Opcode {
    VM1_HALT=0, VM1_MOV_IMM=1, VM1_XOR_IMM=2, VM1_ADD_IMM=3,
    VM1_MUL_IMM=4, VM1_ROL=5, VM1_LOAD_INPUT=6, VM1_INPUT_LENGTH=7,
    VM1_XOR_REG=8, VM1_OR_REG=9, VM1_MOV_REG=10, VM1_ADD_REG=11,
    VM1_CALL_VM2=12, VM1_SHR=13, VM1_JZ_REL8=14
} Vm1Opcode;
typedef enum Vm2Opcode {
    VM2_HALT=0, VM2_MOV_IMM_BE=1, VM2_XOR_IMM_BE=2, VM2_SUB_IMM_BE=3,
    VM2_ROR=4, VM2_LOAD_INPUT=5, VM2_ROL5_XOR_REG=6, VM2_MOV_REG=7,
    VM2_ZERO=8, VM2_CALL_VM3=9, VM2_OR_IMM_BE=10, VM2_SHR=11,
    VM2_OR_REG=12, VM2_ADD_REG=13
} Vm2Opcode;
typedef enum Vm3Opcode {
    VM3_HALT=0, VM3_PUSH_IMM=1, VM3_DUP=2, VM3_SWAP=3,
    VM3_XOR=4, VM3_ADD=5, VM3_ROL=6, VM3_PUSH_INPUT=7,
    VM3_OR=8, VM3_CALL_VM4=9, VM3_DROP=10, VM3_XOR_IMM=11,
    VM3_ADD_IMM=12, VM3_MUL_IMM=13, VM3_ROR=14
} Vm3Opcode;
typedef enum Vm4Opcode {
    VM4_NOP=0, VM4_MOV_IMM_BE=1, VM4_XOR_IMM_BE=2, VM4_MUL_IMM_BE=3,
    VM4_ROL=4, VM4_LOAD_INPUT=5, VM4_XOR_REG=6, VM4_MOV_REG=7,
    VM4_OR_REG=8, VM4_CALL_VM5=9, VM4_RETURN=10, VM4_ZERO=11,
    VM4_ADD_IMM_BE=12, VM4_SHR=13, VM4_ADD_REG=14
} Vm4Opcode;
typedef enum Vm5Opcode {
    VM5_LOAD_INPUT=0x11, VM5_XOR_IMM=0x22, VM5_ADD_IMM=0x33,
    VM5_ROL8=0x44, VM5_MUL8=0x55, VM5_CHECK_BYTE=0x66,
    VM5_CHECK_LENGTH=0x88, VM5_ZERO=0x99, VM5_RETURN=0xFF
} Vm5Opcode;
typedef enum VmValidationError {
    VM_ERROR_NONE=0, VM_ERROR_BYTE_MISMATCH=1, VM_ERROR_LENGTH_MISMATCH=2,
    VM_ERROR_BAD_BYTECODE=0xDEADBEEF
} VmValidationError;
"""
errors = ida_typeinf.parse_decls(None, decls, None, ida_typeinf.PT_SIL)
if errors:
    raise RuntimeError(f"{errors} type declaration errors")

# Repair the unrecognized __chkstk_ms/sub rsp,rax prologue, without patching code.
ida_frame.add_user_stkpnt(0x140008B99, -0x47C8)
ida_frame.set_frame_size(ida_funcs.get_func(MAIN), 0x47C8, 0x40, 0)
name(0x1400026D0, "__chkstk_ms")
signature(0x1400026D0,
          "unsigned __int64 __usercall __chkstk_ms@<rax>(unsigned __int64 allocation_size@<rax>);")
comment(0x1400026D0, "MinGW stack probe: input and preserved result in RAX; touches each 4 KiB page. Not a normal Win64 argument-taking function.")

for ea, nm, decl in [
    (MAIN, "main", "int __cdecl main(int argc, char **argv, char **envp);"),
    (0x140008440, "mingw_acrt_iob_func", "FILE *__cdecl mingw_acrt_iob_func(unsigned int stream_index);"),
    (0x140002710, "mingw_printf", "int __cdecl mingw_printf(const char *format, ...);"),
    (0x140008340, "mingw_lock_file", "void __cdecl mingw_lock_file(FILE *stream);"),
    (0x1400083B0, "mingw_unlock_file", "void __cdecl mingw_unlock_file(FILE *stream);"),
    (0x140004870, "mingw_pformat", "int __cdecl mingw_pformat(unsigned int flags, void *destination, int quota, const char *format, va_list args);"),
    (0x140001560, "mingw_run_global_ctors_once", "void __cdecl mingw_run_global_ctors_once(void);"),
    (0x1400014E0, "mingw_run_global_ctors", "void __cdecl mingw_run_global_ctors(void);"),
]:
    name(ea, nm)
    signature(ea, decl)

for ea, nm, count in [
    (0x14000C1C0, "encrypted_success_message", 71),
    (0x14000C207, "vm5_xor_key", 4),
    (0x14000C220, "vm1_opcode_decode_table", 256),
    (0x14000C320, "vm5_encrypted_bytecode", 269),
    (0x14000C440, "vm4_encrypted_bytecode", 1172),
    (0x14000C8E0, "vm3_encrypted_bytecode", 617),
    (0x14000CB60, "vm2_encrypted_bytecode", 974),
    (0x14000CF40, "vm1_encrypted_bytecode", 1720),
]:
    name(ea, nm)
    signature(ea, f"const vm_u8 {nm}[{count}];")
name(0x14000B0F0, "p_mingw_acrt_iob_func")
signature(0x14000B0F0, "FILE *(__cdecl *p_mingw_acrt_iob_func)(unsigned int stream_index);")
name(0x140011030, "global_ctors_have_run")
signature(0x140011030, "int global_ctors_have_run;")

simd_names = [
    "vm3_decrypt_initial_lanes", "vm4_decrypt_initial_indices",
    "vm3_decrypt_offset_1", "vm3_decrypt_offset_5",
    "vm3_decrypt_offset_9", "vm3_decrypt_offset_13",
    "vm3_decrypt_mix_multiplier", "simd_low_byte_per_word_mask",
    "vm3_decrypt_next_16_stride", "vm4_decrypt_index_plus_4",
    "vm4_decrypt_index_plus_8", "vm4_decrypt_index_plus_12",
    "vm4_decrypt_index_step_16", "vm4_decrypt_shift5_byte_mask",
    "vm4_decrypt_bias_17", "vm4_decrypt_bias_90", "simd_low_nibble_mask",
]
for i,nm in enumerate(simd_names):
    name(0x14000D600+16*i,nm)

blocks = {
    0x140008C50: ("prompt_and_read_password", "Read at most 511 bytes, reject embedded NUL and overflow; handle LF/CRLF. Input is workspace.password."),
    0x140008CDE: ("password_rejected_retry", "Shared VM/reader failure path; print wrong password and prompt again."),
    0x140008D58: ("vm1_decrypt_bytecode", "Decrypt 1720 bytes: s=1103515245*s+12345 (u32), s0=0x15AE2D6F; plaintext[i]=ciphertext[i] ^ ((s>>16)&255)."),
    0x140008DC0: ("vm1_dispatch", "8-register VM. opcode=decode_table[(cipher_op ^ rolling_key)&255]; rolling_key starts 0x81 and updates 33*key+7+opcode. Immediates are little-endian."),
    0x140008DF0: ("vm1_halt_and_check_error", "Accept the VM chain only if VM1 r[0]==0. VM1 r[2] also seeds the success-message decryption."),
    0x140008E30: ("decrypt_success_message_and_hash", "Decrypt 71 message bytes using VM1 r[2]; hash h=33*h+byte must equal 0x16607CC9. Wrong message seed cannot pass."),
    0x140008E8E: ("success_message_checksum_failed", "Final message-integrity failure returns exit status 1."),
    0x140008F80: ("vm1_call_vm2_and_decrypt", "VM1 opcode 12: VM2 r[2]=VM1 r[4]; r[1]=seed^0xC52AFA37. Decrypt 974 bytes with xorshift32(13,17,5), initial 0xA191534F. OR VM2 result into VM1 r[0]."),
    0x1400093F9: ("vm2_call_vm3_and_decrypt", "VM2 opcode 9: initialize VM3 stack with VM2 r[1]. Decrypt 617 bytes using index-based arithmetic; SIMD handles the first 608 bytes."),
    0x140009863: ("vm3_call_vm4", "VM3 opcode 9: use current stack top (or 1 for empty stack) as VM4 r[6], execute VM4, then push VM4 r[0]."),
    0x140009A42: ("password_accepted", "All VM checks and the success-message checksum passed. Print the decrypted message and return 0. Verified password: WL7s3EU9i!bN!264!5qhx_3q"),
    0x140009D26: ("vm4_call_vm5_and_decrypt", "VM4 opcode 9: VM5 error accumulator starts at VM4 r[4]. Decode 269 bytes: ((cipher[i]-3*i)&255) ^ key[i&3] ^ ((93*i)&255). OR VM5 result into VM4 r[0]."),
}
for ea,(nm,text) in blocks.items():
    name(ea,nm)
    comment(ea,text)

opnames = [
    ["halt", "mov_imm", "xor_imm", "add_imm", "mul_imm", "rol", "load_input", "input_length", "xor_reg", "or_reg", "mov_reg", "add_reg", "call_vm2", "shr", "jz_rel8"],
    ["halt", "mov_imm_be", "xor_imm_be", "sub_imm_be", "ror", "load_input", "rol5_xor_reg", "mov_reg", "zero", "call_vm3", "or_imm_be", "shr", "or_reg", "add_reg"],
    ["halt", "push_imm", "dup", "swap", "xor", "add", "rol", "push_input", "or", "call_vm4", "drop", "xor_imm", "add_imm", "mul_imm", "ror"],
    ["nop", "mov_imm_be", "xor_imm_be", "mul_imm_be", "rol", "load_input", "xor_reg", "mov_reg", "or_reg", "call_vm5", "return", "zero", "add_imm_be", "shr", "add_reg"],
]
for vm,dispatch in enumerate([0x140008DEE,0x140009063,0x140009708,0x140009C7D],1):
    si=ida_nalt.get_switch_info(dispatch)
    if not si:
        raise RuntimeError(f"Missing VM{vm} switch")
    name(si.jumps,f"vm{vm}_handler_jump_table")
    cases=ida_xref.calc_switch_cases(dispatch,si)
    comment(dispatch,f"VM{vm} native instruction dispatch. Handler labels correspond to decoded VM opcodes, not separate native functions.")
    for vals,target in zip(cases.cases,cases.targets):
        for val in vals:
            label=f"vm{vm}_op_{opnames[vm-1][val]}"
            # Keep the more descriptive names on nested calls and VM1 halt.
            if target not in blocks:
                name(target,label)
            comment(target,f"VM{vm} opcode {val}: {opnames[vm-1][val]}.")

ida_funcs.set_func_cmt(ida_funcs.get_func(MAIN),
    "Matryoshka crackme: console input and FIVE INLINED VMs (VM1 -> VM2 -> VM3 -> VM4 -> VM5).\n"
    "All eight-register/stack arithmetic is modulo 2^32; VM5 arithmetic is modulo 256.\n"
    "Each VM returns an error word, ORed into its parent's error register. VM5 checks length 24 and bytes.\n"
    "VM1 r[0] must be zero; VM1 r[2] decrypts the success message, whose hash must be 0x16607CC9.\n"
    "Workspace at RSP+0x40; VM3 stack at +0xA0, password[512] at +0x120; decoded VM5/+0x320, VM1/+0x720, VM2/+0x1720, VM3/+0x2720, VM4/+0x3720.\n"
    "Verified password: WL7s3EU9i!bN!264!5qhx_3q. Solved independently by angr native symbolic execution.\n"
    "Stack-probe delta repaired at 0x140008B99; executable bytes are unchanged.",True)
ida_hexrays.mark_cfunc_dirty(MAIN)
ROOT.joinpath("idb_cleanup_names.json").write_text(json.dumps(changes,indent=2))
print(f"Applied {len(changes)} symbol/label renames, type definitions, signatures, and VM comments.")

# One real contiguous workspace, rather than unrelated __int128 scalars that
# alias adjacent register banks. The typed extent ends before XMM register spills.
ida_hexrays.mark_cfunc_dirty(MAIN)
cfunc = ida_hexrays.decompile(MAIN)
workspace_var = next(v for v in cfunc.get_lvars()
                     if v.is_stk_var() and v.location.stkoff() == 0x40)
workspace_type = ida_typeinf.tinfo_t()
assert workspace_type.get_named_type(None, "MatryoshkaWorkspace")
assert workspace_type.get_size() == 0x46E0
info = ida_hexrays.lvar_saved_info_t()
info.ll = ida_hexrays.lvar_locator_t(workspace_var.location, workspace_var.defea)
info.name = "workspace"
info.type = workspace_type
info.size = workspace_type.get_size()
info.cmt = "RSP+0x40: real contiguous VM workspace; password at RSP+0x120."
assert ida_hexrays.modify_user_lvar_info(
    MAIN, ida_hexrays.MLI_NAME | ida_hexrays.MLI_TYPE | ida_hexrays.MLI_CMT, info)
ida_hexrays.mark_cfunc_dirty(MAIN)

local_names = {
    "input_length_1": "password_length",
    "input_length_2": "vm1_input_index",
    "input_length_3": "vm2_input_index",
    "input_length_4": "vm3_input_index",
    "input_length_5": "vm4_input_index",
    "input_length_6": "vm5_input_index",
    "standard_stream": "stdout_on_success",
    "p_mingw_acrt_iob_func": "get_standard_stream",
    "v4": "input_invalid",
    "n511": "input_length",
    "Stream": "stdout_before_input",
    "n10": "input_char",
    "n10_1": "char_after_cr",
    "Character": "pushback_char",
    "Stream_1": "stdin_stream",
    "v12": "input_write_index",
    "vm1_encrypted_bytecode": "vm1_cipher_cursor",
    "vm1_code": "vm1_plain_cursor",
    "n363736431": "vm1_lcg_state",
    "v16": "vm1_opcode_key",
    "n0x6B7": "vm1_pc",
    "v18": "vm1_operand_pc",
    "v19": "vm1_opcode",
    "n375422153": "success_message_hash",
    "encrypted_success_message": "message_cipher_cursor",
    "p_vm4_or_message": "message_plain_cursor",
    "v23": "message_prng_state",
    "v24": "message_mix_word",
    "v25": "vm1_shift_operand_pc",
    "v26": "vm1_shift_reg",
    "v27": "vm2_seed",
    "vm2_encrypted_bytecode": "vm2_cipher_cursor",
    "vm2_code": "vm2_plain_cursor",
    "v30": "vm2_xorshift_state",
    "n0x6B7_2": "vm1_resume_pc",
    "v32": "vm1_saved_opcode_key",
    "n116": "vm2_opcode_key",
    "n0x3CD": "vm2_pc",
    "n0x3CD_1": "vm2_operand_pc",
    "v36": "vm2_decoded_opcode_word",
    "v37": "vm1_add_src_pc",
    "v38": "vm1_add_dst_reg",
    "v39": "vm1_mov_src_pc",
    "v40": "vm1_or_src_pc",
    "v41": "vm1_or_dst_reg",
    "v42": "vm1_xor_src_pc",
    "v43": "vm1_xor_dst_reg",
    "v44": "vm1_input_operand_pc",
    "v45": "vm1_input_byte",
    "n511_2": "vm1_input_index",
    "v47": "vm1_rol_operand_pc",
    "v48": "vm1_rotate_count",
    "v49": "vm1_rotate_reg",
    "v50": "vm1_mul_imm_pc",
    "v51": "vm1_mul_reg",
    "v52": "vm1_add_imm_pc",
    "v53": "vm1_add_reg",
    "v54": "vm1_xor_imm_pc",
    "v55": "vm1_xor_immediate",
    "v56": "vm1_xor_reg",
    "v57": "vm1_mov_imm_pc",
    "v58": "vm2_result",
    "n0x6B7_1": "vm1_pc_after_vm2",
    "v60": "vm2_add_src_pc",
    "v61": "vm2_ror_operand_pc",
    "v62": "vm2_rotate_count",
    "v63": "vm2_rotate_reg",
    "v64": "vm2_xor_imm_pc",
    "v65": "vm2_xor_immediate",
    "v66": "vm2_xor_reg",
    "v67": "vm2_mov_imm_pc",
    "v68": "vm2_sub_imm_pc",
    "v69": "vm2_sub_immediate",
    "v70": "vm2_sub_reg",
    "v71": "vm2_mov_src_pc",
    "v72": "vm2_shift_reg",
    "v73": "vm2_or_src_pc",
    "v74": "vm2_or_reg",
    "v75": "vm2_or_imm_pc",
    "v76": "vm2_or_imm_reg",
    "vm3_encrypted_bytecode": "vm3_cipher_simd_cursor",
    "v78": "vm3_seed",
    "vm3_decrypt_initial_lanes": "vm3_decrypt_lanes",
    "vm3_code": "vm3_plain_simd_cursor",
    "vm3_encrypted_bytecode_1": "vm3_cipher_tail_cursor",
    "v110": "vm3_plain_tail_cursor",
    "i": "vm3_tail_prng_state",
    "v112": "vm3_opcode_key",
    "n0x268": "vm3_pc",
    "n0x1F": "vm3_stack_depth",
    "v115": "vm2_saved_opcode",
    "n0x268_1": "vm3_operand_pc",
    "v117": "vm3_opcode",
    "v118": "vm2_mix_src_pc",
    "v119": "vm2_mix_dst_reg",
    "v120": "vm2_input_operand_pc",
    "v121": "vm2_input_byte",
    "v122": "vm2_input_dst_operand",
    "n511_3": "vm2_input_index",
    "v124": "vm2_copy_src_pc",
    "v125": "vm4_seed",
    "v126": "vm3_or_stack_index",
    "n0x268_2": "vm3_input_operand_pc",
    "v128": "vm3_input_byte",
    "n511_4": "vm3_input_index",
    "v130": "vm3_input_push_index",
    "v131": "vm3_rol_count",
    "v132": "vm3_add_stack_index",
    "v133": "vm3_xor_stack_index",
    "v134": "vm3_swap_top_index",
    "v135": "vm3_swap_next_index",
    "v136": "vm3_swap_temporary",
    "n0x1E": "vm3_dup_source_index",
    "v138": "vm3_dup_value",
    "v139": "vm3_dup_destination_index",
    "v140": "vm3_push_destination_index",
    "v141": "vm3_ror_count",
    "Stream_2": "stdout_on_success",
    "vm4_decrypt_initial_indices": "vm4_decrypt_indices",
    "vm4_encrypted_bytecode": "vm4_cipher_simd_cursor",
    "p_vm4_or_message_1": "vm4_plain_simd_cursor",
    "vm4_decrypt_initial_indices_1": "vm4_decrypt_indices_copy",
    "n116_1": "vm2_key_during_vm4",
    "n0x493": "vm4_pc",
    "n0x493_1": "vm4_operand_pc",
    "n0xEF": "vm4_instruction_word",
    "v173": "vm4_instruction_byte",
    "v174": "vm5_error",
    "v175": "vm4_result",
    "v176": "vm3_result_push_index",
    "v177": "vm5_decode_xor_counter",
    "n269": "vm5_decode_index",
    "vm5_code": "vm5_plain_cursor",
    "v180": "vm5_decode_subtracted_byte",
    "v181": "vm5_decode_key_index",
    "v182": "vm5_plain_byte",
    "j_1": "vm5_next_pc",
    "v184": "vm5_accumulator",
    "j": "vm5_pc",
    "n85": "vm5_opcode",
    "v187": "vm5_operand_pc",
    "n511_5": "vm4_input_index",
    "v189": "vm4_input_byte",
    "v190": "vm4_rotate_count",
    "n511_6": "vm5_input_index",
    "n511_1": "password_length",
    "n0x6B7_3": "vm1_resume_pc_saved",
    "n0x3CD_2": "vm2_resume_pc",
    "v195": "vm2_opcode_saved_on_stack",
    "n0x268_3": "vm3_resume_pc",
    "v197": "vm1_saved_opcode",
    "v198": "vm1_key_saved_on_stack",
    "n116_2": "vm2_key_saved_for_vm5",
}
# SIMD intermediates are arithmetic/packing temporaries, not VM registers.
for i in range(82, 110):
    local_names[f"v{i}"] = f"vm3_decrypt_simd_tmp_{i-81:02}"
for i in range(146, 169):
    local_names.setdefault(f"v{i}", f"vm4_decrypt_simd_tmp_{i-145:02}")

local_types = {
    "message_plain_cursor": "vm_u8 *",
    "vm4_plain_simd_cursor": "vm_u8 *",
    "vm1_lcg_state": "vm_u32",
    "vm2_xorshift_state": "vm_u32",
    "vm5_error": "vm_u32",
    "vm5_accumulator": "vm_u32",
    "vm5_opcode": "Vm5Opcode",
    "vm1_opcode": "Vm1Opcode",
    "vm3_opcode": "Vm3Opcode",
    "vm4_instruction_word": "vm_u32",
}
cfunc = ida_hexrays.decompile(MAIN)
local_changes = []
for var in cfunc.get_lvars():
    new_name = local_names.get(var.name, var.name)
    decl = local_types.get(new_name)
    if new_name == var.name and decl is None:
        continue
    info = ida_hexrays.lvar_saved_info_t()
    info.ll = ida_hexrays.lvar_locator_t(var.location, var.defea)
    flags = 0
    if new_name != var.name:
        info.name = new_name
        flags |= ida_hexrays.MLI_NAME
    if decl:
        tif = ida_typeinf.tinfo_t()
        assert ida_typeinf.parse_decl(tif, None, decl + " __lvar_type;", ida_typeinf.PT_SIL), decl
        info.type = tif
        info.size = tif.get_size()
        flags |= ida_hexrays.MLI_TYPE
    assert ida_hexrays.modify_user_lvar_info(MAIN, flags, info), new_name
    local_changes.append({"old_name": var.name, "name": new_name, "type": decl})
ida_hexrays.mark_cfunc_dirty(MAIN)
cfunc = ida_hexrays.decompile(MAIN)
ROOT.joinpath("main_cleaned.c").write_text(str(cfunc))
ROOT.joinpath("idb_cleanup_locals.json").write_text(json.dumps(local_changes, indent=2))
print(f"Renamed/retyped {len(local_changes)} locals; wrote main_cleaned.c.")
