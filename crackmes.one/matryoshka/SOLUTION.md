# Matryoshka solution

Password:

```text
WL7s3EU9i!bN!264!5qhx_3q
```

Verified against the original, unpatched executable:

```text
[+] Correct! All five virtual machines agree. You found the password.
```

## Reverse engineering

Analysis used the installed `ida_mcp` package to attach to the database already
open in IDA. `ida_bridge.py` invokes its `open_database` and `execute_python`
tools; the harness did not expose these tools directly.

The password prompt references lead to `main` at `0x140008B80`. All five
interpreters are inlined into this function. Each nested interpreter reads the
same input and returns a cumulative error value to its caller. The outermost
error register must be zero.

| VM | Encrypted bytecode | Encoding and architecture |
|---|---|---|
| 1 | `0x14000CF40` | LCG byte-stream XOR; rolling opcode key and substitution table at `0x14000C220`; eight 32-bit registers |
| 2 | `0x14000CB60` | xorshift32 byte-stream XOR; rolling affine opcode decoding; big-endian immediates |
| 3 | `0x14000C8E0` | index-based arithmetic byte-stream XOR; rolling opcode decoding; stack machine |
| 4 | `0x14000C440` | nibble rotation and index-based mask; high opcode nibble selects operation, low bits select register |
| 5 | `0x14000C320` | index arithmetic and four-byte XOR key at `0x14000C207`; byte accumulator |

The innermost VM checks length 24 and applies invertible byte operations
(XOR, addition, rotate, odd multiplication) before comparing each character
against a constant. The outer VMs impose additional 32-bit arithmetic checks
and pass seed values between interpreter layers.

`dump_ida.py` extracts the encrypted bytecode and saves IDA's pseudocode in
`main_decompiled.c`. `solve.py` decrypts the streams, implements their
instruction semantics, and symbolically executes them using Z3. It constrains
each innermost comparison and the final outer error register to zero.

The solver returns the password above; it also finds no alternative password
within its printable-ASCII search domain.

## Reproduce

The extracted data is included in `vm_data.json`. With Python and `z3-solver`:

```sh
python3 solve.py
```

The solver writes `password.txt` and the decoded `vm1.bin`–`vm5.bin` streams.
Run `crackme.exe` on Windows and enter the recovered password.

## Direct native execution with angr

`solve_angr.py` is a separate approach: it loads `crackme.exe` and symbolically
executes the original machine code, including bytecode decryption and all five
interpreters. It does not import `vm_data.json`, decoded bytecode, the previous
password, or the hand-written VM implementation.

Execution begins at `0x140008D58`, immediately after the console line reader.
The script supplies its stack input buffer, a symbolic length in the original
1–511 byte range, and the two table-pointer registers initialized by `main`.
Input bytes exclude NUL, CR, and LF, matching a single nonempty accepted line.
The native code determines the required length and password checks.

Veritesting merges branches. The target is `0x140009A42`, reached only after
the outer VM returns zero and the decrypted success message passes its checksum;
both wrong-password paths are avoided.

```sh
~/.virtualenvs/angr/bin/python solve_angr.py
```

This environment uses angr 10, which exposes Claripy as `angr.claripy`.
The script writes its result to `password_angr.txt`.

Verified run: angr recovered length 24 and `WL7s3EU9i!bN!264!5qhx_3q` in
119.7 seconds. Feeding `password_angr.txt` to the unmodified Windows executable
produced the success message shown above.

## Annotated IDB

`cleanup_idb.py` is an IDAPython script (executed through the ida_mcp
`execute_python` tool) that documents the analysis in `crackme.exe.i64`
without patching any executable bytes:

- Functions named and typed: `main`, `__chkstk_ms`, and the MinGW runtime
  helpers (`mingw_printf`, `mingw_pformat`, `mingw_lock_file`,
  `mingw_unlock_file`, `mingw_acrt_iob_func`, global-constructor runners).
- A `user_stkpnt` repairs stack tracking across the `__chkstk_ms` prologue, so
  `main` decompiles without the "positive sp value" warning and its frame is
  consistent.
- Types: `vm_u8`/`vm_u32`, `VmRegisterFile32`, `Vm4CodeOrMessage`,
  `MatryoshkaWorkspace` (the RSP+0x40 workspace), and opcode enums
  `Vm1Opcode`..`Vm5Opcode` plus `VmValidationError`.
- Encrypted byte streams, the opcode decode table, the VM5 XOR key, the
  encrypted success message, the SIMD constant table, and the indirect
  `__iob_func` pointer are named and sized.
- All four switch dispatches are labeled `vmN_handler_jump_table`, with each
  handler named for its VM opcode (`vm1_op_mov_imm`, ..., `vm4_call_vm5`) and
  commented. Block comments document the bytecode decoders, the register-file
  saves/restores around nested calls, and the final checksum path.
- Hex-Rays locals are renamed to readable names (`password_length`, `vmN_pc`,
  `vmN_opcode`, `vmN_opcode_key`, `vmN_operand_pc`, `decrypt_simd_tmp_*`, ...),
  and `workspace` is typed as `MatryoshkaWorkspace`.
- `main` carries a summary function comment; a full annotated pseudocode dump
  was saved to `main_cleaned.c`.

Rename and retyping records: `idb_cleanup_names.json` (105 symbols) and
`idb_cleanup_locals.json` (178 locals).
