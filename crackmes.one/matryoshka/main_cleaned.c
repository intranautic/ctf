// Matryoshka crackme: console input and FIVE INLINED VMs (VM1 -> VM2 -> VM3 -> VM4 -> VM5).
// All eight-register/stack arithmetic is modulo 2^32; VM5 arithmetic is modulo 256.
// Each VM returns an error word, ORed into its parent's error register. VM5 checks length 24 and bytes.
// VM1 r[0] must be zero; VM1 r[2] decrypts the success message, whose hash must be 0x16607CC9.
// Workspace at RSP+0x40; VM3 stack at +0xA0, password[512] at +0x120; decoded VM5/+0x320, VM1/+0x720, VM2/+0x1720, VM3/+0x2720, VM4/+0x3720.
// Verified password: WL7s3EU9i!bN!264!5qhx_3q. Solved independently by angr native symbolic execution.
// Stack-probe delta repaired at 0x140008B99; executable bytes are unchanged.
int __fastcall main(int argc, char **argv, char **envp)
{
  FILE *(__fastcall *get_standard_stream)(unsigned int); // r14
  char input_invalid; // bl
  unsigned int input_length; // ebp
  FILE *stdout_before_input; // rax
  int input_char; // eax
  int char_after_cr; // eax
  int pushback_char; // esi
  FILE *stdin_stream; // rax
  __int64 input_write_index; // rdx
  const vm_u8 *vm1_cipher_cursor; // r8
  vm_u8 *vm1_plain_cursor; // rdx
  vm_u32 vm1_lcg_state; // eax
  __int32 vm1_opcode_key; // r11d
  unsigned int vm1_pc; // eax
  __int64 vm1_operand_pc; // rbx
  Vm1Opcode vm1_opcode; // r10d
  int success_message_hash; // edx
  const vm_u8 *message_cipher_cursor; // r9
  vm_u8 *message_plain_cursor; // r8
  vm_u32 message_prng_state; // ecx
  int message_mix_word; // eax
  __int64 vm1_shift_operand_pc; // rcx
  __int64 vm1_shift_reg; // rdx
  vm_u32 vm2_seed; // edx
  const vm_u8 *vm2_cipher_cursor; // r8
  vm_u8 *vm2_plain_cursor; // rcx
  vm_u32 vm2_xorshift_state; // esi
  unsigned int vm1_resume_pc; // ebp
  __int32 vm1_saved_opcode_key; // r9d
  int vm2_opcode_key; // esi
  unsigned int vm2_pc; // eax
  __int64 vm2_operand_pc; // r11
  int vm2_decoded_opcode_word; // edx
  __int64 vm1_add_src_pc; // rcx
  __int64 vm1_add_dst_reg; // rdx
  __int64 vm1_mov_src_pc; // rdx
  __int64 vm1_or_src_pc; // rcx
  __int64 vm1_or_dst_reg; // rdx
  __int64 vm1_xor_src_pc; // rcx
  __int64 vm1_xor_dst_reg; // rdx
  __int64 vm1_input_operand_pc; // rcx
  vm_u32 vm1_input_byte; // r8d
  __int64 vm1_input_index; // r9
  __int64 vm1_rol_operand_pc; // rdx
  char vm1_rotate_count; // cl
  __int64 vm1_rotate_reg; // rdx
  __int64 vm1_mul_imm_pc; // r8
  __int64 vm1_mul_reg; // rdx
  __int64 vm1_add_imm_pc; // rcx
  __int64 vm1_add_reg; // rdx
  __int64 vm1_xor_imm_pc; // rdx
  int vm1_xor_immediate; // ecx
  __int64 vm1_xor_reg; // rdx
  __int64 vm1_mov_imm_pc; // rdx
  vm_u32 vm2_result; // eax
  unsigned int vm1_pc_after_vm2; // ebx
  __int64 vm2_add_src_pc; // r8
  __int64 vm2_ror_operand_pc; // rcx
  char vm2_rotate_count; // cl
  __int64 vm2_rotate_reg; // r8
  __int64 vm2_xor_imm_pc; // rcx
  unsigned __int32 vm2_xor_immediate; // r8d
  __int64 vm2_xor_reg; // rcx
  __int64 vm2_mov_imm_pc; // rcx
  __int64 vm2_sub_imm_pc; // rcx
  unsigned __int32 vm2_sub_immediate; // r8d
  __int64 vm2_sub_reg; // rcx
  __int64 vm2_mov_src_pc; // r8
  __int64 vm2_shift_reg; // rcx
  __int64 vm2_or_src_pc; // rcx
  __int64 vm2_or_reg; // r8
  __int64 vm2_or_imm_pc; // rcx
  __int64 vm2_or_imm_reg; // r8
  const vm_u8 *vm3_cipher_simd_cursor; // rax
  vm_u32 vm3_seed; // edi
  __m128i vm3_decrypt_lanes; // xmm4
  vm_u8 *vm3_plain_simd_cursor; // r8
  const vm_u8 *vm3_cipher_tail_cursor; // rcx
  __m128i vm3_decrypt_simd_tmp_01; // xmm0
  __m128i vm3_decrypt_simd_tmp_02; // xmm2
  __m128i vm3_decrypt_simd_tmp_03; // xmm13
  __m128i vm3_decrypt_simd_tmp_04; // xmm11
  __m128i vm3_decrypt_simd_tmp_05; // xmm0
  __m128i vm3_decrypt_simd_tmp_06; // xmm0
  __m128i vm3_decrypt_simd_tmp_07; // xmm12
  __m128i vm3_decrypt_simd_tmp_08; // xmm12
  __m128i vm3_decrypt_simd_tmp_09; // xmm1
  __m128i vm3_decrypt_simd_tmp_10; // xmm14
  __m128i vm3_decrypt_simd_tmp_11; // xmm1
  __m128i vm3_decrypt_simd_tmp_12; // xmm11
  __m128i vm3_decrypt_simd_tmp_13; // xmm11
  __m128i vm3_decrypt_simd_tmp_14; // xmm13
  __m128i vm3_decrypt_simd_tmp_15; // xmm15
  __m128i vm3_decrypt_simd_tmp_16; // xmm2
  __m128i vm3_decrypt_simd_tmp_17; // xmm13
  __m128i vm3_decrypt_simd_tmp_18; // xmm2
  __m128i vm3_decrypt_simd_tmp_19; // xmm14
  __m128i vm3_decrypt_simd_tmp_20; // xmm13
  __m128i vm3_decrypt_simd_tmp_21; // xmm14
  __m128i vm3_decrypt_simd_tmp_22; // xmm2
  __m128i vm3_decrypt_simd_tmp_23; // xmm13
  __m128i vm3_decrypt_simd_tmp_24; // xmm0
  __m128i vm3_decrypt_simd_tmp_25; // xmm13
  __m128i vm3_decrypt_simd_tmp_26; // xmm12
  __m128i vm3_decrypt_simd_tmp_27; // xmm1
  __m128i vm3_decrypt_simd_tmp_28; // xmm12
  vm_u8 *vm3_plain_tail_cursor; // r10
  unsigned int vm3_tail_prng_state; // r8d
  char vm3_opcode_key; // r13
  unsigned int vm3_pc; // eax
  unsigned int vm3_stack_depth; // edi
  int vm2_saved_opcode; // ebp
  __int64 vm3_operand_pc; // r8
  Vm3Opcode vm3_opcode; // edx
  __int64 vm2_mix_src_pc; // rcx
  __int64 vm2_mix_dst_reg; // r8
  __int64 vm2_input_operand_pc; // rcx
  vm_u32 vm2_input_byte; // r10d
  vm_u8 vm2_input_dst_operand; // r8
  __int64 vm2_input_index; // r11
  __int64 vm2_copy_src_pc; // rcx
  vm_u32 vm4_seed; // r9d
  __int64 vm3_or_stack_index; // rax
  __int64 vm3_input_operand_pc; // rcx
  vm_u32 vm3_input_byte; // r8d
  __int64 vm3_input_index; // r10
  __int64 vm3_input_push_index; // rcx
  char vm3_rol_count; // cl
  __int64 vm3_add_stack_index; // rcx
  __int64 vm3_xor_stack_index; // rax
  __int64 vm3_swap_top_index; // rcx
  __int64 vm3_swap_next_index; // rax
  vm_u32 vm3_swap_temporary; // r10d
  __int64 vm3_dup_source_index; // rax
  vm_u32 vm3_dup_value; // ecx
  __int64 vm3_dup_destination_index; // rax
  __int64 vm3_push_destination_index; // rcx
  char vm3_ror_count; // cl
  FILE *stdout_on_success; // rax
  __m128i vm4_decrypt_indices; // xmm2
  const vm_u8 *vm4_cipher_simd_cursor; // rcx
  vm_u8 *vm4_plain_simd_cursor; // rax
  __m128i vm4_decrypt_simd_tmp_01; // xmm12
  __m128i vm4_decrypt_simd_tmp_02; // xmm13
  __m128i vm4_decrypt_simd_tmp_03; // xmm1
  __m128i vm4_decrypt_simd_tmp_04; // xmm12
  __m128i vm4_decrypt_simd_tmp_05; // xmm0
  __m128i vm4_decrypt_simd_tmp_06; // xmm12
  __m128i vm4_decrypt_indices_copy; // xmm1
  __m128i vm4_decrypt_simd_tmp_08; // xmm0
  __m128i vm4_decrypt_simd_tmp_09; // xmm12
  __m128i vm4_decrypt_simd_tmp_10; // xmm1
  __m128i vm4_decrypt_simd_tmp_11; // xmm12
  __m128i vm4_decrypt_simd_tmp_12; // xmm0
  __m128i vm4_decrypt_simd_tmp_13; // xmm12
  __m128i vm4_decrypt_simd_tmp_14; // xmm13
  __m128i vm4_decrypt_simd_tmp_15; // xmm12
  __m128i vm4_decrypt_simd_tmp_16; // xmm13
  __m128i vm4_decrypt_simd_tmp_17; // xmm12
  __m128i vm4_decrypt_simd_tmp_18; // xmm12
  __m128i vm4_decrypt_simd_tmp_19; // xmm0
  __m128i vm4_decrypt_simd_tmp_20; // xmm12
  __m128i vm4_decrypt_simd_tmp_21; // xmm1
  __m128i vm4_decrypt_simd_tmp_22; // xmm1
  __m128i vm4_decrypt_simd_tmp_23; // xmm1
  int vm2_key_during_vm4; // r10d
  unsigned int vm4_pc; // r8d
  __int64 vm4_operand_pc; // r11
  vm_u32 vm4_instruction_word; // ecx
  vm_u8 vm4_instruction_byte; // al
  vm_u32 vm5_error; // ebp
  vm_u32 vm4_result; // eax
  __int64 vm3_result_push_index; // rcx
  int vm5_decode_xor_counter; // r8d
  __int64 vm5_decode_index; // rcx
  vm_u8 *vm5_plain_cursor; // r9
  char vm5_decode_subtracted_byte; // al
  char vm5_decode_key_index; // r10
  vm_u8 vm5_plain_byte; // al
  unsigned int vm5_next_pc; // r9d
  vm_u32 vm5_accumulator; // r8d
  __int64 vm5_pc; // rsi
  Vm5Opcode vm5_opcode; // eax
  __int64 vm5_operand_pc; // rcx
  __int64 vm4_input_index; // r11
  vm_u32 vm4_input_byte; // r9d
  char vm4_rotate_count; // cl
  __int64 vm5_input_index; // rcx
  unsigned int password_length; // [rsp+24h] [rbp-47A4h]
  unsigned int vm1_resume_pc_saved; // [rsp+28h] [rbp-47A0h]
  int vm2_resume_pc; // [rsp+30h] [rbp-4798h]
  int vm2_opcode_saved_on_stack; // [rsp+34h] [rbp-4794h]
  int vm3_resume_pc; // [rsp+38h] [rbp-4790h]
  unsigned __int8 vm1_saved_opcode; // [rsp+3Dh] [rbp-478Bh]
  unsigned __int8 vm1_key_saved_on_stack; // [rsp+3Eh] [rbp-478Ah]
  unsigned __int8 vm2_key_saved_for_vm5; // [rsp+3Fh] [rbp-4789h]
  MatryoshkaWorkspace workspace; // [rsp+40h] [rbp-4788h] BYREF RSP+0x40: real contiguous VM workspace; password at RSP+0x120.

  mingw_run_global_ctors_once();
  puts("====================================================");
  puts("   Crackme - five nested virtual machines");
  puts("   Goal: find the password.");
  puts("====================================================");
  puts(&Buffer_);
  get_standard_stream = p_mingw_acrt_iob_func;
prompt_and_read_password:
  input_invalid = 0;
  input_length = 0;
  mingw_printf("Enter password: ");
  stdout_before_input = get_standard_stream(1u);
  fflush(stdout_before_input);
  while ( 1 )
  {
    input_char = getchar();
    if ( input_char == 10 || input_char == -1 )
      break;
    if ( input_char == 13 )
    {
      char_after_cr = getchar();
      pushback_char = char_after_cr;
      if ( char_after_cr != 10 && char_after_cr != -1 )
      {
        stdin_stream = get_standard_stream(0);
        ungetc(pushback_char, stdin_stream);
      }
      workspace.password[input_length] = 0;
LABEL_12:
      if ( input_invalid == 0 )
      {
        vm1_cipher_cursor = vm1_encrypted_bytecode;
        vm1_plain_cursor = workspace.vm1_code;
        vm1_lcg_state = 363736431;
        do
        {
          ++vm1_plain_cursor;
          ++vm1_cipher_cursor;
          vm1_lcg_state = 1103515245 * vm1_lcg_state + 12345;
          *(vm1_plain_cursor - 1) = *(vm1_cipher_cursor - 1) ^ BYTE2(vm1_lcg_state);
        }
        while ( vm1_plain_cursor != &workspace.vm1_code[1720] );
        password_length = input_length;
        vm1_opcode_key = -127;
        vm1_pc = 0;
        memset(&workspace, 0, 32);
        do
        {
          vm1_operand_pc = vm1_pc + 1;
          vm1_opcode = vm1_opcode_decode_table[(unsigned __int8)(vm1_opcode_key ^ workspace.vm1_code[vm1_pc])];
          switch ( (char)vm1_opcode )
          {
            case VM1_HALT:
              success_message_hash = workspace.vm1.r[0];
              if ( workspace.vm1.r[0] != 0 )
                goto password_rejected_retry;
              message_cipher_cursor = encrypted_success_message;
              message_plain_cursor = workspace.vm4_or_message.bytecode;
              message_prng_state = (workspace.vm1.r[2] ^ 0xCBFCDF6E) - 1640531527;
              do
              {
                ++message_plain_cursor;
                ++message_cipher_cursor;
                message_mix_word = message_prng_state ^ HIWORD(message_prng_state);
                message_prng_state -= 1640531527;
                LOBYTE(message_mix_word) = *(message_cipher_cursor - 1)
                                         ^ ((1935289751
                                           * (((unsigned int)(569420461 * message_mix_word) >> 15)
                                            ^ (569420461 * message_mix_word))) >> 24);
                *(message_plain_cursor - 1) = message_mix_word;
                success_message_hash = (unsigned __int8)message_mix_word + 33 * success_message_hash;
              }
              while ( message_plain_cursor != &workspace.vm4_or_message.allocation[71] );
              workspace.vm4_or_message.bytecode[71] = 0;
              if ( success_message_hash == 375422153 )
              {
                stdout_on_success = get_standard_stream(1u);
                fputs((const char *)&workspace.vm4_or_message, stdout_on_success);
                return 0;
              }
              else
              {
                puts(&Buffer_);
                puts("[-] Wrong password.");
                return 1;
              }
            case VM1_MOV_IMM:
              vm1_mov_imm_pc = vm1_pc + 2;
              vm1_pc += 6;
              workspace.vm1.r[workspace.vm1_code[(unsigned int)vm1_operand_pc] & 7] = *(_DWORD *)&workspace.vm1_code[vm1_mov_imm_pc];
              goto LABEL_30;
            case VM1_XOR_IMM:
              vm1_xor_imm_pc = vm1_pc + 2;
              vm1_pc += 6;
              vm1_xor_immediate = *(_DWORD *)&workspace.vm1_code[vm1_xor_imm_pc];
              vm1_xor_reg = workspace.vm1_code[(unsigned int)vm1_operand_pc] & 7;
              workspace.vm1.r[vm1_xor_reg] ^= vm1_xor_immediate;
              goto LABEL_30;
            case VM1_ADD_IMM:
              vm1_add_imm_pc = vm1_pc + 2;
              vm1_pc += 6;
              vm1_add_reg = workspace.vm1_code[(unsigned int)vm1_operand_pc] & 7;
              workspace.vm1.r[vm1_add_reg] += *(_DWORD *)&workspace.vm1_code[vm1_add_imm_pc];
              goto LABEL_30;
            case VM1_MUL_IMM:
              vm1_mul_imm_pc = vm1_pc + 2;
              vm1_pc += 6;
              vm1_mul_reg = workspace.vm1_code[(unsigned int)vm1_operand_pc] & 7;
              workspace.vm1.r[vm1_mul_reg] *= *(_DWORD *)&workspace.vm1_code[vm1_mul_imm_pc];
              goto LABEL_30;
            case VM1_ROL:
              vm1_rol_operand_pc = vm1_pc + 2;
              vm1_pc += 3;
              vm1_rotate_count = workspace.vm1_code[vm1_rol_operand_pc];
              if ( (vm1_rotate_count & 0x1F) != 0 )
              {
                vm1_rotate_reg = workspace.vm1_code[(unsigned int)vm1_operand_pc] & 7;
                workspace.vm1.r[vm1_rotate_reg] = __ROL4__(workspace.vm1.r[vm1_rotate_reg], vm1_rotate_count);
              }
              goto LABEL_30;
            case VM1_LOAD_INPUT:
              vm1_input_operand_pc = vm1_pc + 2;
              vm1_pc += 3;
              vm1_input_byte = 0;
              vm1_input_index = workspace.vm1_code[vm1_input_operand_pc];
              if ( (unsigned int)vm1_input_index < password_length )
                vm1_input_byte = workspace.password[vm1_input_index];
              workspace.vm1.r[workspace.vm1_code[(unsigned int)vm1_operand_pc] & 7] = vm1_input_byte;
              goto LABEL_30;
            case VM1_INPUT_LENGTH:
              vm1_pc += 2;
              workspace.vm1.r[workspace.vm1_code[(unsigned int)vm1_operand_pc] & 7] = password_length;
              goto LABEL_30;
            case VM1_XOR_REG:
              vm1_xor_src_pc = vm1_pc + 2;
              vm1_pc += 3;
              vm1_xor_dst_reg = workspace.vm1_code[(unsigned int)vm1_operand_pc] & 7;
              workspace.vm1.r[vm1_xor_dst_reg] ^= workspace.vm1.r[workspace.vm1_code[vm1_xor_src_pc] & 7];
              goto LABEL_30;
            case VM1_OR_REG:
              vm1_or_src_pc = vm1_pc + 2;
              vm1_pc += 3;
              vm1_or_dst_reg = workspace.vm1_code[(unsigned int)vm1_operand_pc] & 7;
              workspace.vm1.r[vm1_or_dst_reg] |= workspace.vm1.r[workspace.vm1_code[vm1_or_src_pc] & 7];
              goto LABEL_30;
            case VM1_MOV_REG:
              vm1_mov_src_pc = vm1_pc + 2;
              vm1_pc += 3;
              workspace.vm1.r[workspace.vm1_code[(unsigned int)vm1_operand_pc] & 7] = workspace.vm1.r[workspace.vm1_code[vm1_mov_src_pc] & 7];
              goto LABEL_30;
            case VM1_ADD_REG:
              vm1_add_src_pc = vm1_pc + 2;
              vm1_pc += 3;
              vm1_add_dst_reg = workspace.vm1_code[(unsigned int)vm1_operand_pc] & 7;
              workspace.vm1.r[vm1_add_dst_reg] += workspace.vm1.r[workspace.vm1_code[vm1_add_src_pc] & 7];
              goto LABEL_30;
            case VM1_CALL_VM2:
              vm2_seed = workspace.vm1.r[4];
              vm2_cipher_cursor = vm2_encrypted_bytecode;
              vm2_plain_cursor = workspace.vm2_code;
              vm2_xorshift_state = 0xA191534F;
              do
              {
                ++vm2_plain_cursor;
                ++vm2_cipher_cursor;
                vm2_xorshift_state ^= ((vm2_xorshift_state ^ (vm2_xorshift_state << 13)) >> 17)
                                    ^ (vm2_xorshift_state << 13)
                                    ^ (32
                                     * (((vm2_xorshift_state ^ (vm2_xorshift_state << 13)) >> 17)
                                      ^ vm2_xorshift_state
                                      ^ (vm2_xorshift_state << 13)));
                *(vm2_plain_cursor - 1) = vm2_xorshift_state ^ *(vm2_cipher_cursor - 1);
              }
              while ( &workspace.vm2_code[974] != vm2_plain_cursor );
              workspace.vm2.r[0] = 0;
              vm1_resume_pc = vm1_pc + 1;
              vm1_saved_opcode_key = vm1_opcode_key;
              workspace.vm2.r[2] = vm2_seed;
              vm2_opcode_key = 116;
              memset(&workspace.vm2.r[3], 0, 20);
              vm1_saved_opcode = vm1_opcode;
              workspace.vm2.r[1] = vm2_seed ^ 0xC52AFA37;
              vm2_pc = 0;
              break;
            case VM1_SHR:
              vm1_shift_operand_pc = vm1_pc + 2;
              vm1_pc += 3;
              vm1_shift_reg = workspace.vm1_code[(unsigned int)vm1_operand_pc] & 7;
              workspace.vm1.r[vm1_shift_reg] >>= workspace.vm1_code[vm1_shift_operand_pc];
              goto LABEL_30;
            case VM1_JZ_REL8:
              if ( workspace.vm1.r[workspace.vm1_code[vm1_operand_pc] & 7] != 0 )
                vm1_pc += 3;
              else
                vm1_pc += (char)workspace.vm1_code[vm1_pc + 2] + 3;
              goto LABEL_30;
            default:
              goto password_rejected_retry;
          }
          do
          {
            vm2_operand_pc = vm2_pc + 1;
            vm2_decoded_opcode_word = 101 * (unsigned __int8)((vm2_opcode_key ^ workspace.vm2_code[vm2_pc]) - 59);
            switch ( (char)vm2_decoded_opcode_word )
            {
              case 0:
                vm1_opcode = vm1_saved_opcode;
                vm2_result = workspace.vm2.r[0];
                vm1_pc_after_vm2 = vm1_resume_pc;
                vm1_opcode_key = vm1_saved_opcode_key;
                goto LABEL_53;
              case 1:
                vm2_mov_imm_pc = vm2_pc + 2;
                vm2_pc += 6;
                workspace.vm2.r[workspace.vm2_code[(unsigned int)vm2_operand_pc] & 7] = _byteswap_ulong(*(_DWORD *)&workspace.vm2_code[vm2_mov_imm_pc]);
                goto LABEL_55;
              case 2:
                vm2_xor_imm_pc = vm2_pc + 2;
                vm2_pc += 6;
                vm2_xor_immediate = _byteswap_ulong(*(_DWORD *)&workspace.vm2_code[vm2_xor_imm_pc]);
                vm2_xor_reg = workspace.vm2_code[(unsigned int)vm2_operand_pc] & 7;
                workspace.vm2.r[vm2_xor_reg] ^= vm2_xor_immediate;
                goto LABEL_55;
              case 3:
                vm2_sub_imm_pc = vm2_pc + 2;
                vm2_pc += 6;
                vm2_sub_immediate = _byteswap_ulong(*(_DWORD *)&workspace.vm2_code[vm2_sub_imm_pc]);
                vm2_sub_reg = workspace.vm2_code[(unsigned int)vm2_operand_pc] & 7;
                workspace.vm2.r[vm2_sub_reg] -= vm2_sub_immediate;
                goto LABEL_55;
              case 4:
                vm2_ror_operand_pc = vm2_pc + 2;
                vm2_pc += 3;
                vm2_rotate_count = workspace.vm2_code[vm2_ror_operand_pc];
                if ( (vm2_rotate_count & 0x1F) != 0 )
                {
                  vm2_rotate_reg = workspace.vm2_code[(unsigned int)vm2_operand_pc] & 7;
                  workspace.vm2.r[vm2_rotate_reg] = __ROR4__(workspace.vm2.r[vm2_rotate_reg], vm2_rotate_count);
                }
                goto LABEL_55;
              case 5:
                vm2_input_operand_pc = vm2_pc + 2;
                vm2_input_byte = 0;
                vm2_pc += 3;
                vm2_input_dst_operand = workspace.vm2_code[(unsigned int)vm2_operand_pc];
                vm2_input_index = workspace.vm2_code[vm2_input_operand_pc];
                if ( (unsigned int)vm2_input_index < password_length )
                  vm2_input_byte = workspace.password[vm2_input_index];
                workspace.vm2.r[vm2_input_dst_operand & 7] = vm2_input_byte;
                goto LABEL_55;
              case 6:
                vm2_mix_src_pc = vm2_pc + 2;
                vm2_pc += 3;
                vm2_mix_dst_reg = workspace.vm2_code[(unsigned int)vm2_operand_pc] & 7;
                workspace.vm2.r[vm2_mix_dst_reg] = workspace.vm2.r[workspace.vm2_code[vm2_mix_src_pc] & 7]
                                                 ^ __ROL4__(workspace.vm2.r[vm2_mix_dst_reg], 5);
                goto LABEL_55;
              case 7:
                vm2_copy_src_pc = vm2_pc + 2;
                vm2_pc += 3;
                workspace.vm2.r[workspace.vm2_code[(unsigned int)vm2_operand_pc] & 7] = workspace.vm2.r[workspace.vm2_code[vm2_copy_src_pc] & 7];
                goto LABEL_55;
              case 8:
                vm2_pc += 2;
                workspace.vm2.r[workspace.vm2_code[(unsigned int)vm2_operand_pc] & 7] = 0;
                goto LABEL_55;
              case 9:
                vm3_cipher_simd_cursor = vm3_encrypted_bytecode;
                vm3_seed = workspace.vm2.r[1];
                vm3_decrypt_lanes = (__m128i)vm3_decrypt_initial_lanes;
                vm3_plain_simd_cursor = workspace.vm3_code;
                vm3_cipher_tail_cursor = &vm3_encrypted_bytecode[608];
                do
                {
                  vm3_cipher_simd_cursor += 16;
                  vm3_plain_simd_cursor += 16;
                  vm3_decrypt_simd_tmp_01 = _mm_add_epi32(vm3_decrypt_lanes, (__m128i)vm3_decrypt_offset_1);
                  vm3_decrypt_simd_tmp_02 = _mm_add_epi32(vm3_decrypt_lanes, (__m128i)vm3_decrypt_offset_5);
                  vm3_decrypt_simd_tmp_03 = _mm_add_epi32(vm3_decrypt_lanes, (__m128i)vm3_decrypt_offset_9);
                  vm3_decrypt_simd_tmp_04 = _mm_add_epi32(vm3_decrypt_lanes, (__m128i)vm3_decrypt_offset_13);
                  vm3_decrypt_lanes = _mm_add_epi32(vm3_decrypt_lanes, (__m128i)vm3_decrypt_next_16_stride);
                  vm3_decrypt_simd_tmp_05 = _mm_xor_si128(
                                              _mm_srli_epi32(vm3_decrypt_simd_tmp_01, 0x10u),
                                              vm3_decrypt_simd_tmp_01);
                  vm3_decrypt_simd_tmp_06 = _mm_unpacklo_epi32(
                                              _mm_shuffle_epi32(
                                                _mm_mul_epu32(
                                                  vm3_decrypt_simd_tmp_05,
                                                  (__m128i)vm3_decrypt_mix_multiplier),
                                                8),
                                              _mm_shuffle_epi32(
                                                _mm_mul_epu32(
                                                  _mm_srli_epi64(vm3_decrypt_simd_tmp_05, 0x20u),
                                                  (__m128i)vm3_decrypt_mix_multiplier),
                                                8));
                  vm3_decrypt_simd_tmp_07 = _mm_xor_si128(
                                              _mm_srli_epi32(vm3_decrypt_simd_tmp_02, 0x10u),
                                              vm3_decrypt_simd_tmp_02);
                  vm3_decrypt_simd_tmp_08 = _mm_unpacklo_epi32(
                                              _mm_shuffle_epi32(
                                                _mm_mul_epu32(
                                                  vm3_decrypt_simd_tmp_07,
                                                  (__m128i)vm3_decrypt_mix_multiplier),
                                                8),
                                              _mm_shuffle_epi32(
                                                _mm_mul_epu32(
                                                  _mm_srli_epi64(vm3_decrypt_simd_tmp_07, 0x20u),
                                                  (__m128i)vm3_decrypt_mix_multiplier),
                                                8));
                  vm3_decrypt_simd_tmp_09 = _mm_xor_si128(
                                              _mm_srli_epi32(vm3_decrypt_simd_tmp_03, 0x10u),
                                              vm3_decrypt_simd_tmp_03);
                  vm3_decrypt_simd_tmp_10 = _mm_srli_epi32(vm3_decrypt_simd_tmp_08, 0xFu);
                  vm3_decrypt_simd_tmp_11 = _mm_unpacklo_epi32(
                                              _mm_shuffle_epi32(
                                                _mm_mul_epu32(
                                                  vm3_decrypt_simd_tmp_09,
                                                  (__m128i)vm3_decrypt_mix_multiplier),
                                                8),
                                              _mm_shuffle_epi32(
                                                _mm_mul_epu32(
                                                  _mm_srli_epi64(vm3_decrypt_simd_tmp_09, 0x20u),
                                                  (__m128i)vm3_decrypt_mix_multiplier),
                                                8));
                  vm3_decrypt_simd_tmp_12 = _mm_xor_si128(
                                              _mm_srli_epi32(vm3_decrypt_simd_tmp_04, 0x10u),
                                              vm3_decrypt_simd_tmp_04);
                  vm3_decrypt_simd_tmp_13 = _mm_unpacklo_epi32(
                                              _mm_shuffle_epi32(
                                                _mm_mul_epu32(
                                                  vm3_decrypt_simd_tmp_12,
                                                  (__m128i)vm3_decrypt_mix_multiplier),
                                                8),
                                              _mm_shuffle_epi32(
                                                _mm_mul_epu32(
                                                  _mm_srli_epi64(vm3_decrypt_simd_tmp_12, 0x20u),
                                                  (__m128i)vm3_decrypt_mix_multiplier),
                                                8));
                  vm3_decrypt_simd_tmp_14 = _mm_srli_epi32(vm3_decrypt_simd_tmp_06, 0xFu);
                  vm3_decrypt_simd_tmp_15 = _mm_srli_epi32(vm3_decrypt_simd_tmp_13, 0xFu);
                  vm3_decrypt_simd_tmp_16 = _mm_unpacklo_epi16(vm3_decrypt_simd_tmp_14, vm3_decrypt_simd_tmp_10);
                  vm3_decrypt_simd_tmp_17 = _mm_unpackhi_epi16(vm3_decrypt_simd_tmp_14, vm3_decrypt_simd_tmp_10);
                  vm3_decrypt_simd_tmp_18 = _mm_unpacklo_epi16(
                                              _mm_unpacklo_epi16(vm3_decrypt_simd_tmp_16, vm3_decrypt_simd_tmp_17),
                                              _mm_unpackhi_epi16(vm3_decrypt_simd_tmp_16, vm3_decrypt_simd_tmp_17));
                  vm3_decrypt_simd_tmp_19 = _mm_srli_epi32(vm3_decrypt_simd_tmp_11, 0xFu);
                  vm3_decrypt_simd_tmp_20 = _mm_unpacklo_epi16(vm3_decrypt_simd_tmp_19, vm3_decrypt_simd_tmp_15);
                  vm3_decrypt_simd_tmp_21 = _mm_unpackhi_epi16(vm3_decrypt_simd_tmp_19, vm3_decrypt_simd_tmp_15);
                  vm3_decrypt_simd_tmp_22 = _mm_packus_epi16(
                                              _mm_and_si128(
                                                vm3_decrypt_simd_tmp_18,
                                                (__m128i)simd_low_byte_per_word_mask),
                                              _mm_and_si128(
                                                _mm_unpacklo_epi16(
                                                  _mm_unpacklo_epi16(vm3_decrypt_simd_tmp_20, vm3_decrypt_simd_tmp_21),
                                                  _mm_unpackhi_epi16(vm3_decrypt_simd_tmp_20, vm3_decrypt_simd_tmp_21)),
                                                (__m128i)simd_low_byte_per_word_mask));
                  vm3_decrypt_simd_tmp_23 = vm3_decrypt_simd_tmp_06;
                  vm3_decrypt_simd_tmp_24 = _mm_unpacklo_epi16(vm3_decrypt_simd_tmp_06, vm3_decrypt_simd_tmp_08);
                  vm3_decrypt_simd_tmp_25 = _mm_unpackhi_epi16(vm3_decrypt_simd_tmp_23, vm3_decrypt_simd_tmp_08);
                  vm3_decrypt_simd_tmp_26 = vm3_decrypt_simd_tmp_11;
                  vm3_decrypt_simd_tmp_27 = _mm_unpacklo_epi16(vm3_decrypt_simd_tmp_11, vm3_decrypt_simd_tmp_13);
                  vm3_decrypt_simd_tmp_28 = _mm_unpackhi_epi16(vm3_decrypt_simd_tmp_26, vm3_decrypt_simd_tmp_13);
                  *((__m128i *)vm3_plain_simd_cursor - 1) = _mm_xor_si128(
                                                              vm3_decrypt_simd_tmp_22,
                                                              _mm_xor_si128(
                                                                _mm_packus_epi16(
                                                                  _mm_and_si128(
                                                                    _mm_unpacklo_epi16(
                                                                      _mm_unpacklo_epi16(
                                                                        vm3_decrypt_simd_tmp_24,
                                                                        vm3_decrypt_simd_tmp_25),
                                                                      _mm_unpackhi_epi16(
                                                                        vm3_decrypt_simd_tmp_24,
                                                                        vm3_decrypt_simd_tmp_25)),
                                                                    (__m128i)simd_low_byte_per_word_mask),
                                                                  _mm_and_si128(
                                                                    _mm_unpacklo_epi16(
                                                                      _mm_unpacklo_epi16(
                                                                        vm3_decrypt_simd_tmp_27,
                                                                        vm3_decrypt_simd_tmp_28),
                                                                      _mm_unpackhi_epi16(
                                                                        vm3_decrypt_simd_tmp_27,
                                                                        vm3_decrypt_simd_tmp_28)),
                                                                    (__m128i)simd_low_byte_per_word_mask)),
                                                                *((__m128i *)vm3_cipher_simd_cursor - 1)));
                }
                while ( vm3_cipher_tail_cursor != vm3_cipher_simd_cursor );
                vm3_plain_tail_cursor = &workspace.vm3_code[608];
                for ( vm3_tail_prng_state = 0xBB4E05E2;
                      vm3_tail_prng_state != 1262570851;
                      *(vm3_plain_tail_cursor - 1) = *(vm3_cipher_tail_cursor - 1)
                                                   ^ ((569420461 * (vm3_tail_prng_state ^ HIWORD(vm3_tail_prng_state))) >> 15)
                                                   ^ (-83 * (vm3_tail_prng_state ^ BYTE2(vm3_tail_prng_state))) )
                {
                  vm3_tail_prng_state -= 1640531527;
                  ++vm3_cipher_tail_cursor;
                  ++vm3_plain_tail_cursor;
                }
                vm1_key_saved_on_stack = vm1_saved_opcode_key;
                vm3_opcode_key = -81;
                vm3_pc = 0;
                workspace.vm3_stack[0] = vm3_seed;
                vm3_stack_depth = 1;
                vm1_resume_pc_saved = vm1_resume_pc;
                vm2_saved_opcode = vm2_decoded_opcode_word;
                break;
              case 10:
                vm2_or_imm_pc = vm2_pc + 2;
                vm2_pc += 6;
                vm2_or_imm_reg = workspace.vm2_code[(unsigned int)vm2_operand_pc] & 7;
                workspace.vm2.r[vm2_or_imm_reg] |= (workspace.vm2_code[vm2_or_imm_pc + 2] << 8)
                                                 | (workspace.vm2_code[vm2_or_imm_pc + 1] << 16)
                                                 | (workspace.vm2_code[vm2_or_imm_pc] << 24)
                                                 | workspace.vm2_code[vm2_or_imm_pc + 3];
                goto LABEL_55;
              case 11:
                vm2_or_src_pc = vm2_pc + 2;
                vm2_pc += 3;
                vm2_or_reg = workspace.vm2_code[(unsigned int)vm2_operand_pc] & 7;
                workspace.vm2.r[vm2_or_reg] >>= workspace.vm2_code[vm2_or_src_pc];
                goto LABEL_55;
              case 12:
                vm2_mov_src_pc = vm2_pc + 2;
                vm2_pc += 3;
                vm2_shift_reg = workspace.vm2_code[(unsigned int)vm2_operand_pc] & 7;
                workspace.vm2.r[vm2_shift_reg] |= workspace.vm2.r[workspace.vm2_code[vm2_mov_src_pc] & 7];
                goto LABEL_55;
              case 13:
                vm2_add_src_pc = vm2_pc + 2;
                vm2_pc += 3;
                workspace.vm2.r[workspace.vm2_code[vm2_operand_pc] & 7] += workspace.vm2.r[workspace.vm2_code[vm2_add_src_pc]
                                                                                         & 7];
                goto LABEL_55;
              default:
                goto LABEL_56;
            }
            do
            {
              vm3_operand_pc = vm3_pc + 1;
              vm3_opcode = 255 - (unsigned __int8)(vm3_opcode_key ^ workspace.vm3_code[vm3_pc]);
              switch ( vm3_opcode )
              {
                case VM3_HALT:
                  vm2_decoded_opcode_word = vm2_saved_opcode;
                  vm1_saved_opcode_key = vm1_key_saved_on_stack;
                  vm1_resume_pc = vm1_resume_pc_saved;
                  if ( vm3_stack_depth != 0 )
                    vm3_stack_depth = workspace.vm3_stack[vm3_stack_depth - 1];
                  goto LABEL_111;
                case VM3_PUSH_IMM:
                  vm3_pc += 5;
                  if ( vm3_stack_depth <= 0x1F )
                  {
                    vm3_push_destination_index = vm3_stack_depth++;
                    workspace.vm3_stack[vm3_push_destination_index] = *(_DWORD *)&workspace.vm3_code[vm3_operand_pc];
                  }
                  goto LABEL_79;
                case VM3_DUP:
                  vm3_dup_source_index = vm3_stack_depth - 1;
                  if ( (unsigned int)vm3_dup_source_index > 0x1E )
                    goto LABEL_104;
                  vm3_dup_value = workspace.vm3_stack[vm3_dup_source_index];
                  vm3_dup_destination_index = vm3_stack_depth++;
                  workspace.vm3_stack[vm3_dup_destination_index] = vm3_dup_value;
                  vm3_pc = vm3_operand_pc;
                  goto LABEL_79;
                case VM3_SWAP:
                  ++vm3_pc;
                  if ( vm3_stack_depth <= 1 )
                    goto LABEL_79;
                  vm3_swap_top_index = vm3_stack_depth - 1;
                  vm3_swap_next_index = vm3_stack_depth - 2;
                  vm3_swap_temporary = workspace.vm3_stack[vm3_swap_top_index];
                  workspace.vm3_stack[vm3_swap_top_index] = workspace.vm3_stack[vm3_swap_next_index];
                  workspace.vm3_stack[vm3_swap_next_index] = vm3_swap_temporary;
                  goto LABEL_104;
                case VM3_XOR:
                  if ( vm3_stack_depth <= 1 )
                    goto LABEL_104;
                  vm3_xor_stack_index = vm3_stack_depth - 2;
                  workspace.vm3_stack[vm3_xor_stack_index] ^= workspace.vm3_stack[--vm3_stack_depth];
                  vm3_pc = vm3_operand_pc;
                  goto LABEL_79;
                case VM3_ADD:
                  ++vm3_pc;
                  if ( vm3_stack_depth > 1 )
                  {
                    vm3_add_stack_index = vm3_stack_depth - 2;
                    workspace.vm3_stack[vm3_add_stack_index] += workspace.vm3_stack[--vm3_stack_depth];
                  }
                  goto LABEL_79;
                case VM3_ROL:
                  vm3_pc += 2;
                  vm3_rol_count = workspace.vm3_code[(unsigned int)vm3_operand_pc];
                  if ( (vm3_rol_count & 0x1F) != 0 && vm3_stack_depth != 0 )
                    workspace.vm3_stack[vm3_stack_depth - 1] = __ROL4__(
                                                                 workspace.vm3_stack[vm3_stack_depth - 1],
                                                                 vm3_rol_count);
                  goto LABEL_79;
                case VM3_PUSH_INPUT:
                  vm3_pc += 2;
                  if ( vm3_stack_depth <= 0x1F )
                  {
                    vm3_input_operand_pc = (unsigned int)vm3_operand_pc;
                    vm3_input_byte = 0;
                    vm3_input_index = workspace.vm3_code[vm3_input_operand_pc];
                    if ( (unsigned int)vm3_input_index < password_length )
                      vm3_input_byte = workspace.password[vm3_input_index];
                    vm3_input_push_index = vm3_stack_depth++;
                    workspace.vm3_stack[vm3_input_push_index] = vm3_input_byte;
                  }
                  goto LABEL_79;
                case VM3_OR:
                  if ( vm3_stack_depth <= 1 )
                    goto LABEL_104;
                  vm3_or_stack_index = vm3_stack_depth - 2;
                  workspace.vm3_stack[vm3_or_stack_index] |= workspace.vm3_stack[--vm3_stack_depth];
                  vm3_pc = vm3_operand_pc;
                  goto LABEL_79;
                case VM3_CALL_VM4:
                  if ( vm3_stack_depth != 0 )
                  {
                    ++vm3_pc;
                    if ( vm3_stack_depth > 0x1F )
                      goto LABEL_79;
                    vm4_seed = workspace.vm3_stack[vm3_stack_depth - 1];
                  }
                  else
                  {
                    vm4_seed = 1;
                  }
                  vm4_decrypt_indices = (__m128i)vm4_decrypt_initial_indices;
                  vm4_cipher_simd_cursor = vm4_encrypted_bytecode;
                  vm4_plain_simd_cursor = workspace.vm4_or_message.bytecode;
                  do
                  {
                    vm4_plain_simd_cursor += 16;
                    vm4_cipher_simd_cursor += 16;
                    vm4_decrypt_simd_tmp_01 = _mm_add_epi32(vm4_decrypt_indices, (__m128i)vm4_decrypt_index_plus_4);
                    vm4_decrypt_simd_tmp_02 = _mm_add_epi32(vm4_decrypt_indices, (__m128i)vm4_decrypt_index_plus_12);
                    vm4_decrypt_simd_tmp_03 = _mm_unpackhi_epi16(vm4_decrypt_indices, vm4_decrypt_simd_tmp_01);
                    vm4_decrypt_simd_tmp_04 = _mm_unpacklo_epi16(vm4_decrypt_indices, vm4_decrypt_simd_tmp_01);
                    vm4_decrypt_simd_tmp_05 = _mm_unpacklo_epi16(vm4_decrypt_simd_tmp_04, vm4_decrypt_simd_tmp_03);
                    vm4_decrypt_simd_tmp_06 = _mm_unpackhi_epi16(vm4_decrypt_simd_tmp_04, vm4_decrypt_simd_tmp_03);
                    vm4_decrypt_indices_copy = vm4_decrypt_indices;
                    vm4_decrypt_indices = _mm_add_epi32(vm4_decrypt_indices, (__m128i)vm4_decrypt_index_step_16);
                    vm4_decrypt_simd_tmp_08 = _mm_unpacklo_epi16(vm4_decrypt_simd_tmp_05, vm4_decrypt_simd_tmp_06);
                    vm4_decrypt_simd_tmp_09 = _mm_add_epi32(vm4_decrypt_indices_copy, (__m128i)vm4_decrypt_index_plus_8);
                    vm4_decrypt_simd_tmp_10 = _mm_unpacklo_epi16(vm4_decrypt_simd_tmp_09, vm4_decrypt_simd_tmp_02);
                    vm4_decrypt_simd_tmp_11 = _mm_unpackhi_epi16(vm4_decrypt_simd_tmp_09, vm4_decrypt_simd_tmp_02);
                    vm4_decrypt_simd_tmp_12 = _mm_packus_epi16(
                                                _mm_and_si128(
                                                  vm4_decrypt_simd_tmp_08,
                                                  (__m128i)simd_low_byte_per_word_mask),
                                                _mm_and_si128(
                                                  _mm_unpacklo_epi16(
                                                    _mm_unpacklo_epi16(vm4_decrypt_simd_tmp_10, vm4_decrypt_simd_tmp_11),
                                                    _mm_unpackhi_epi16(vm4_decrypt_simd_tmp_10, vm4_decrypt_simd_tmp_11)),
                                                  (__m128i)simd_low_byte_per_word_mask));
                    vm4_decrypt_simd_tmp_13 = _mm_unpacklo_epi8(vm4_decrypt_simd_tmp_12, vm4_decrypt_simd_tmp_12);
                    vm4_decrypt_simd_tmp_14 = _mm_unpackhi_epi8(vm4_decrypt_simd_tmp_12, vm4_decrypt_simd_tmp_12);
                    vm4_decrypt_simd_tmp_15 = _mm_packus_epi16(
                                                _mm_and_si128(
                                                  _mm_mullo_epi16(vm4_decrypt_simd_tmp_13, vm4_decrypt_simd_tmp_13),
                                                  (__m128i)simd_low_byte_per_word_mask),
                                                _mm_and_si128(
                                                  _mm_mullo_epi16(vm4_decrypt_simd_tmp_14, vm4_decrypt_simd_tmp_14),
                                                  (__m128i)simd_low_byte_per_word_mask));
                    vm4_decrypt_simd_tmp_16 = _mm_sub_epi8(
                                                _mm_and_si128(
                                                  _mm_slli_epi16(vm4_decrypt_simd_tmp_15, 5u),
                                                  (__m128i)vm4_decrypt_shift5_byte_mask),
                                                vm4_decrypt_simd_tmp_15);
                    vm4_decrypt_simd_tmp_17 = _mm_add_epi8(vm4_decrypt_simd_tmp_12, vm4_decrypt_simd_tmp_12);
                    vm4_decrypt_simd_tmp_18 = _mm_add_epi8(vm4_decrypt_simd_tmp_17, vm4_decrypt_simd_tmp_17);
                    vm4_decrypt_simd_tmp_19 = _mm_xor_si128(
                                                _mm_add_epi8(vm4_decrypt_simd_tmp_16, (__m128i)vm4_decrypt_bias_17),
                                                _mm_add_epi8(
                                                  _mm_sub_epi8(
                                                    _mm_add_epi8(vm4_decrypt_simd_tmp_18, vm4_decrypt_simd_tmp_18),
                                                    vm4_decrypt_simd_tmp_12),
                                                  (__m128i)vm4_decrypt_bias_90));
                    vm4_decrypt_simd_tmp_20 = *((__m128i *)vm4_cipher_simd_cursor - 1);
                    vm4_decrypt_simd_tmp_21 = _mm_add_epi8(vm4_decrypt_simd_tmp_20, vm4_decrypt_simd_tmp_20);
                    vm4_decrypt_simd_tmp_22 = _mm_add_epi8(vm4_decrypt_simd_tmp_21, vm4_decrypt_simd_tmp_21);
                    vm4_decrypt_simd_tmp_23 = _mm_add_epi8(vm4_decrypt_simd_tmp_22, vm4_decrypt_simd_tmp_22);
                    *((__m128i *)vm4_plain_simd_cursor - 1) = _mm_xor_si128(
                                                                vm4_decrypt_simd_tmp_19,
                                                                _mm_or_si128(
                                                                  _mm_and_si128(
                                                                    _mm_srli_epi16(vm4_decrypt_simd_tmp_20, 4u),
                                                                    (__m128i)simd_low_nibble_mask),
                                                                  _mm_add_epi8(
                                                                    vm4_decrypt_simd_tmp_23,
                                                                    vm4_decrypt_simd_tmp_23)));
                  }
                  while ( &workspace.vm4_or_message.allocation[1168] != vm4_plain_simd_cursor );
                  *(_DWORD *)&workspace.vm4_or_message.allocation[1168] = -1601173888;
                  workspace.vm4.r[7] = 0;
                  workspace.vm4.r[6] = vm4_seed;
                  vm2_resume_pc = vm2_operand_pc;
                  vm2_opcode_saved_on_stack = vm2_saved_opcode;
                  vm2_key_during_vm4 = vm2_opcode_key;
                  vm3_resume_pc = vm3_operand_pc;
                  vm4_pc = 0;
                  memset(&workspace.vm4, 0, 24);
                  break;
                case VM3_DROP:
                  ++vm3_pc;
                  vm3_stack_depth = (vm3_stack_depth == 0) + vm3_stack_depth - 1;
                  goto LABEL_79;
                case VM3_XOR_IMM:
                  vm3_pc += 5;
                  if ( vm3_stack_depth != 0 )
                    workspace.vm3_stack[vm3_stack_depth - 1] ^= *(_DWORD *)&workspace.vm3_code[vm3_operand_pc];
                  goto LABEL_79;
                case VM3_ADD_IMM:
                  vm3_pc += 5;
                  if ( vm3_stack_depth != 0 )
                    workspace.vm3_stack[vm3_stack_depth - 1] += *(_DWORD *)&workspace.vm3_code[vm3_operand_pc];
                  goto LABEL_79;
                case VM3_MUL_IMM:
                  vm3_pc += 5;
                  if ( vm3_stack_depth != 0 )
                    workspace.vm3_stack[vm3_stack_depth - 1] *= *(_DWORD *)&workspace.vm3_code[vm3_operand_pc];
                  goto LABEL_79;
                case VM3_ROR:
                  vm3_ror_count = workspace.vm3_code[vm3_operand_pc];
                  vm3_pc += 2;
                  if ( (vm3_ror_count & 0x1F) != 0 && vm3_stack_depth != 0 )
                    workspace.vm3_stack[vm3_stack_depth - 1] = __ROR4__(
                                                                 workspace.vm3_stack[vm3_stack_depth - 1],
                                                                 vm3_ror_count);
                  goto LABEL_79;
                default:
                  goto LABEL_80;
              }
              do
              {
                vm4_operand_pc = vm4_pc + 1;
                vm4_instruction_word = workspace.vm4_or_message.bytecode[vm4_pc];
                vm4_instruction_byte = workspace.vm4_or_message.bytecode[vm4_pc];
                if ( vm4_instruction_word > 0xEF )
                  break;
                switch ( vm4_instruction_word >> 4 )
                {
                  case 0u:
                    goto vm4_op_nop;
                  case 1u:
                    vm4_pc += 5;
                    workspace.vm4.r[vm4_instruction_byte & 7] = _byteswap_ulong(*(_DWORD *)&workspace.vm4_or_message.bytecode[vm4_operand_pc]);
                    continue;
                  case 2u:
                    vm4_pc += 5;
                    workspace.vm4.r[vm4_instruction_byte & 7] ^= _byteswap_ulong(*(_DWORD *)&workspace.vm4_or_message.bytecode[vm4_operand_pc]);
                    continue;
                  case 3u:
                    vm4_pc += 5;
                    workspace.vm4.r[vm4_instruction_byte & 7] *= _byteswap_ulong(*(_DWORD *)&workspace.vm4_or_message.bytecode[vm4_operand_pc]);
                    continue;
                  case 4u:
                    vm4_rotate_count = workspace.vm4_or_message.success_message[vm4_operand_pc];
                    vm4_pc += 2;
                    if ( (vm4_rotate_count & 0x1F) != 0 )
                      workspace.vm4.r[vm4_instruction_byte & 7] = __ROL4__(
                                                                    workspace.vm4.r[vm4_instruction_byte & 7],
                                                                    vm4_rotate_count);
                    continue;
                  case 5u:
                    vm4_input_index = workspace.vm4_or_message.bytecode[vm4_operand_pc];
                    vm4_pc += 2;
                    vm4_input_byte = 0;
                    if ( (unsigned int)vm4_input_index < password_length )
                      vm4_input_byte = workspace.password[vm4_input_index];
                    workspace.vm4.r[vm4_instruction_byte & 7] = vm4_input_byte;
                    continue;
                  case 6u:
                    vm4_pc += 2;
                    workspace.vm4.r[vm4_instruction_byte & 7] ^= workspace.vm4.r[workspace.vm4_or_message.bytecode[vm4_operand_pc]
                                                                               & 7];
                    continue;
                  case 7u:
                    vm4_pc += 2;
                    workspace.vm4.r[vm4_instruction_byte & 7] = workspace.vm4.r[workspace.vm4_or_message.bytecode[vm4_operand_pc]
                                                                              & 7];
                    continue;
                  case 8u:
                    vm4_pc += 2;
                    workspace.vm4.r[vm4_instruction_byte & 7] |= workspace.vm4.r[workspace.vm4_or_message.bytecode[vm4_operand_pc]
                                                                               & 7];
                    continue;
                  case 9u:
                    vm2_key_saved_for_vm5 = vm2_key_during_vm4;
                    vm5_error = workspace.vm4.r[4];
                    vm5_decode_xor_counter = 0;
                    vm5_decode_index = 0;
                    vm5_plain_cursor = workspace.vm5_code;
                    do
                    {
                      ++vm5_plain_cursor;
                      vm5_decode_subtracted_byte = vm5_encrypted_bytecode[vm5_decode_index] - 3 * vm5_decode_index;
                      vm5_decode_key_index = vm5_decode_index++;
                      vm5_plain_byte = vm5_decode_xor_counter
                                     ^ vm5_xor_key[vm5_decode_key_index & 3]
                                     ^ vm5_decode_subtracted_byte;
                      vm5_decode_xor_counter += 93;
                      *(vm5_plain_cursor - 1) = vm5_plain_byte;
                    }
                    while ( vm5_decode_index != 269 );
                    vm2_key_during_vm4 = vm2_key_saved_for_vm5;
                    vm5_next_pc = 0;
                    vm5_accumulator = 0;
                    break;
                  case 0xAu:
                    LODWORD(vm2_operand_pc) = vm2_resume_pc;
                    vm2_opcode_key = vm2_key_during_vm4;
                    vm2_saved_opcode = vm2_opcode_saved_on_stack;
                    LODWORD(vm3_operand_pc) = vm3_resume_pc;
                    vm4_result = workspace.vm4.r[0];
                    goto LABEL_133;
                  case 0xBu:
                    workspace.vm4.r[workspace.vm4_or_message.bytecode[vm4_pc] & 7] = 0;
                    goto vm4_op_nop;
                  case 0xCu:
                    vm4_pc += 5;
                    workspace.vm4.r[vm4_instruction_byte & 7] += _byteswap_ulong(*(_DWORD *)&workspace.vm4_or_message.bytecode[vm4_operand_pc]);
                    continue;
                  case 0xDu:
                    vm4_pc += 2;
                    workspace.vm4.r[vm4_instruction_byte & 7] >>= workspace.vm4_or_message.success_message[vm4_operand_pc];
                    continue;
                  case 0xEu:
                    vm4_pc += 2;
                    workspace.vm4.r[vm4_instruction_byte & 7] += workspace.vm4.r[workspace.vm4_or_message.bytecode[vm4_operand_pc]
                                                                               & 7];
                    continue;
                  default:
                    abort();
                }
LABEL_137:
                while ( 1 )
                {
                  for ( vm5_pc = vm5_next_pc; ; ++vm5_pc )
                  {
                    if ( (unsigned int)vm5_pc > 0x10C )
                      goto LABEL_175;
                    LOBYTE(vm5_opcode) = workspace.vm5_code[vm5_pc];
                    vm5_operand_pc = (unsigned int)(vm5_pc + 1);
                    if ( (_BYTE)vm5_opcode == VM5_MUL8 )
                    {
                      vm5_next_pc = vm5_pc + 2;
                      vm5_accumulator = (unsigned __int8)(vm5_accumulator * workspace.vm5_code[vm5_operand_pc]);
                      goto LABEL_137;
                    }
                    if ( (unsigned __int8)vm5_opcode <= (unsigned int)VM5_MUL8 )
                    {
                      if ( (_BYTE)vm5_opcode == VM5_ADD_IMM )
                      {
                        vm5_next_pc = vm5_pc + 2;
                        vm5_accumulator = (unsigned __int8)(workspace.vm5_code[vm5_operand_pc] + vm5_accumulator);
                        goto LABEL_137;
                      }
                      if ( (unsigned __int8)vm5_opcode > (unsigned int)VM5_ADD_IMM )
                      {
                        if ( (_BYTE)vm5_opcode == VM5_ROL8 )
                        {
                          vm5_next_pc = vm5_pc + 2;
                          vm5_accumulator = (unsigned __int8)(((_BYTE)vm5_accumulator << (workspace.vm5_code[vm5_operand_pc]
                                                                                        & 7))
                                                            | (vm5_accumulator >> (-(workspace.vm5_code[vm5_operand_pc]
                                                                                   & 7)
                                                                                 & 7)));
                          goto LABEL_137;
                        }
                      }
                      else
                      {
                        if ( (_BYTE)vm5_opcode == VM5_LOAD_INPUT )
                        {
                          vm5_input_index = workspace.vm5_code[vm5_operand_pc];
                          vm5_next_pc = vm5_pc + 2;
                          vm5_accumulator = 0;
                          if ( (unsigned int)vm5_input_index < password_length )
                            vm5_accumulator = workspace.password[vm5_input_index];
                          goto LABEL_137;
                        }
                        if ( (_BYTE)vm5_opcode == VM5_XOR_IMM )
                        {
                          vm5_next_pc = vm5_pc + 2;
                          vm5_accumulator ^= workspace.vm5_code[vm5_operand_pc];
                          goto LABEL_137;
                        }
                      }
                      goto LABEL_175;
                    }
                    if ( (_BYTE)vm5_opcode != VM5_ZERO )
                      break;
                    vm5_accumulator = 0;
                  }
                  if ( (unsigned __int8)vm5_opcode > (unsigned int)VM5_ZERO )
                    break;
                  if ( (_BYTE)vm5_opcode != VM5_CHECK_BYTE )
                  {
                    if ( (_BYTE)vm5_opcode == VM5_CHECK_LENGTH )
                    {
                      vm5_next_pc = vm5_pc + 2;
                      if ( password_length != workspace.vm5_code[vm5_operand_pc] )
                        vm5_error |= 2u;
                      continue;
                    }
LABEL_175:
                    vm5_error = 0xDEADBEEF;
                    goto LABEL_125;
                  }
                  vm5_next_pc = vm5_pc + 2;
                  if ( vm5_accumulator != workspace.vm5_code[vm5_operand_pc] )
                    vm5_error |= 1u;
                }
                if ( (_BYTE)vm5_opcode != VM5_RETURN )
                  vm5_error = 0xDEADBEEF;
LABEL_125:
                workspace.vm4.r[0] |= vm5_error;
vm4_op_nop:
                vm4_pc = vm4_operand_pc;
              }
              while ( vm4_pc <= 0x493 );
              vm2_saved_opcode = vm2_opcode_saved_on_stack;
              vm2_opcode_key = vm2_key_during_vm4;
              vm4_result = 0xDEADBEEF;
              LODWORD(vm2_operand_pc) = vm2_resume_pc;
              LODWORD(vm3_operand_pc) = vm3_resume_pc;
LABEL_133:
              vm3_result_push_index = vm3_stack_depth++;
              workspace.vm3_stack[vm3_result_push_index] = vm4_result;
LABEL_104:
              vm3_pc = vm3_operand_pc;
LABEL_79:
              vm3_opcode_key = 13 * vm3_opcode_key + 5 + vm3_opcode;
            }
            while ( vm3_pc <= 0x268 );
LABEL_80:
            vm2_decoded_opcode_word = vm2_saved_opcode;
            vm1_saved_opcode_key = vm1_key_saved_on_stack;
            vm1_resume_pc = vm1_resume_pc_saved;
            vm3_stack_depth = 0xDEADBEEF;
LABEL_111:
            workspace.vm2.r[0] |= vm3_stack_depth;
            vm2_pc = vm2_operand_pc;
LABEL_55:
            vm2_opcode_key = 17 * vm2_opcode_key + 3 + vm2_decoded_opcode_word;
          }
          while ( vm2_pc <= 0x3CD );
LABEL_56:
          vm1_opcode = vm1_saved_opcode;
          vm1_pc_after_vm2 = vm1_resume_pc;
          vm1_opcode_key = vm1_saved_opcode_key;
          vm2_result = 0xDEADBEEF;
LABEL_53:
          workspace.vm1.r[0] |= vm2_result;
          vm1_pc = vm1_pc_after_vm2;
LABEL_30:
          vm1_opcode_key = vm1_opcode + 33 * vm1_opcode_key + 7;
        }
        while ( vm1_pc <= 0x6B7 );
      }
password_rejected_retry:
      puts("[-] Wrong password.");
      puts(&Buffer_);
      goto prompt_and_read_password;
    }
    if ( input_length == 511 )
    {
      input_invalid = 1;
    }
    else
    {
      input_invalid |= input_char == 0;
      input_write_index = input_length++;
      workspace.password[input_write_index] = input_char;
    }
  }
  workspace.password[input_length] = 0;
  if ( input_length != 0 || input_char != -1 )
    goto LABEL_12;
  if ( input_invalid != 0 )
    goto password_rejected_retry;
  puts(&Buffer_);
  puts("[-] No input received. Bye!");
  return 1;
}
