import json
import struct
from pathlib import Path
import z3

D = json.loads(Path("vm_data.json").read_text())
def blob(ea): return bytes.fromhex(D[hex(ea)])
M = 0xffffffff
def u(x): return x & M
def rol(x,n): return z3.RotateLeft(x,n) if z3.is_bv(x) else u((x << (n&31)) | (x >> ((-n)&31)))
def ror(x,n): return rol(x,(-n)&31)
def shr(x,n): return z3.LShR(x,n) if z3.is_bv(x) else x >> n
def imm(b,p,endian="little"): return int.from_bytes(b[p:p+4],endian)

# Decode the five independently encrypted bytecode streams.
s=363736431
b1=[]
for c in blob(0x14000cf40):
    s=u(s*1103515245+12345)
    b1.append(c ^ ((s>>16)&255))
s=u(-1584311473)
b2=[]
for c in blob(0x14000cb60):
    s=u(s ^ (s<<13)); s=u(s ^ (s>>17)); s=u(s ^ (s<<5))
    b2.append(c ^ (s&255))
b3=[]
for i,c in enumerate(blob(0x14000c8e0)):
    x=u(4153208450+(i+1)*2654435769)
    x=u((x^(x>>16))*569420461)
    b3.append(c ^ ((x ^ (x>>15))&255))
b4=[]
for i,c in enumerate(blob(0x14000c440)):
    b4.append(((c<<4)|(c>>4))&255 ^ (((31*i*i+17)&255)^((7*i+90)&255)))
b4[1168:1172]=struct.pack("<I",u(-1601173888))
key=blob(0x14000c207)
b5=[((c-3*i)&255)^key[i&3]^((93*i)&255) for i,c in enumerate(blob(0x14000c320))]
for i,b in enumerate([b1,b2,b3,b4,b5],1):
    Path(f"vm{i}.bin").write_bytes(bytes(b))
print("VM5", bytes(b5).hex())

solver=z3.Solver()
password=[z3.BitVec(f"p{i}",32) for i in range(128)]
length=z3.BitVec("length",32)
solver.add(z3.ULE(length,128))
for x in password: solver.add(z3.UGE(x,0x20),z3.ULE(x,0x7e))
def load(i): return z3.If(z3.ULT(i,length),password[i],z3.BitVecVal(0,32))
checks=[]
def check(expr,label):
    checks.append((expr,label))
    solver.add(expr == 0)

def vm5(seed):
    p=0; a=0; err=seed
    while True:
        op=b5[p]; p+=1
        if op==255: return err
        if op==0x99: a=0; continue
        n=b5[p]; p+=1
        if op==0x11: a=load(n)
        elif op==0x22: a=a^n
        elif op==0x33: a=(a+n)&255
        elif op==0x44: a=((a<<(n&7))|shr(a,(-n)&7))&255
        elif op==0x55: a=(a*n)&255
        elif op==0x66:
            check(a^n,f"vm5 byte at {p}")
            # Each mismatch is ORed into the cumulative error.
        elif op==0x88: solver.add(length==n); print("Password length:", n)
        else: raise ValueError(("vm5",p,op))

def vm4(seed):
    r=[0]*8; r[6]=seed; p=0
    while True:
        at=p; raw=b4[p]; op=raw>>4; d=raw&7; p+=1
        if op==0 or op==11:
            if op==11: r[d]=0
        elif op in [1,2,3,12]:
            n=imm(b4,p,"big"); p+=4
            if op==1:r[d]=n
            elif op==2:r[d]^=n
            elif op==3:r[d]=u(r[d]*n)
            else:r[d]=u(r[d]+n)
        elif op in [4,5,6,7,8,13,14]:
            n=b4[p];p+=1
            if op==4:r[d]=rol(r[d],n)
            elif op==5:r[d]=load(n)
            elif op==6:r[d]^=r[n&7]
            elif op==7:r[d]=r[n&7]
            elif op==8:r[d]|=r[n&7]
            elif op==13:r[d]=shr(r[d],n)
            else:r[d]=u(r[d]+r[n&7])
        elif op==9:r[0]|=vm5(r[4])
        elif op==10:return r[0]
        else:raise ValueError(("vm4",hex(at),hex(raw)))

def vm3(seed):
    stack=[seed];p=0; state=175
    while True:
        at=p;op=255-(state^b3[p]);p+=1
        if op==0:return stack[-1] if stack else 0
        elif op in [1,11,12,13]:
            n=imm(b3,p);p+=4
            if op==1:stack.append(n)
            elif op==11:stack[-1]^=n
            elif op==12:stack[-1]=u(stack[-1]+n)
            else:stack[-1]=u(stack[-1]*n)
        elif op==2:stack.append(stack[-1])
        elif op==3:stack[-1],stack[-2]=stack[-2],stack[-1]
        elif op in [4,5,8]:
            x=stack.pop()
            if op==4:stack[-1]^=x
            elif op==5:stack[-1]=u(stack[-1]+x)
            else:stack[-1]|=x
        elif op in [6,7,14]:
            n=b3[p];p+=1
            if op==6:stack[-1]=rol(stack[-1],n)
            elif op==7:stack.append(load(n))
            else:stack[-1]=ror(stack[-1],n)
        elif op==9:stack.append(vm4(stack[-1] if stack else 1))
        elif op==10:stack.pop()
        else:raise ValueError(("vm3",hex(at),op,state,b3[at]))
        state=(13*state+5+op)&255

def vm2(seed):
    r=[0]*8; r[2]=seed;r[1]=seed^0xc52afa37
    p=0;state=116
    while True:
        at=p;op=(101*((state^b2[p])-59))&255;p+=1
        if op==0:return r[0]
        if op in [1,2,3,10]:
            d=b2[p]&7;n=imm(b2,p+1,"big");p+=5
            if op==1:r[d]=n
            elif op==2:r[d]^=n
            elif op==3:r[d]=u(r[d]-n)
            else:r[d]|=n
        elif op in [4,5,6,7,11,12,13]:
            d=b2[p]&7;n=b2[p+1];p+=2
            if op==4:r[d]=ror(r[d],n)
            elif op==5:r[d]=load(n)
            elif op==6:r[d]=r[n&7]^rol(r[d],5)
            elif op==7:r[d]=r[n&7]
            elif op==11:r[d]=shr(r[d],n)
            elif op==12:r[d]|=r[n&7]
            else:r[d]=u(r[d]+r[n&7])
        elif op==8:r[b2[p]&7]=0;p+=1
        elif op==9:r[0]|=vm3(r[1])
        else:raise ValueError(("vm2",hex(at),op))
        state=(17*state+3+op)&255

def vm1():
    r=[0]*8;p=0;state=129;table=blob(0x14000c220)
    while True:
        at=p;op=table[state^b1[p]];p+=1
        if op==0:return r
        if op in [1,2,3,4]:
            d=b1[p]&7;n=imm(b1,p+1);p+=5
            if op==1:r[d]=n
            elif op==2:r[d]^=n
            elif op==3:r[d]=u(r[d]+n)
            else:r[d]=u(r[d]*n)
        elif op in [5,6,8,9,10,11,13,14]:
            d=b1[p]&7;n=b1[p+1];p+=2
            if op==5:r[d]=rol(r[d],n)
            elif op==6:r[d]=load(n)
            elif op==8:r[d]^=r[n&7]
            elif op==9:r[d]|=r[n&7]
            elif op==10:r[d]=r[n&7]
            elif op==11:r[d]=u(r[d]+r[n&7])
            elif op==13:r[d]=shr(r[d],n)
            else:
                print("Branch",at,r[d])
                assert not z3.is_bv(r[d])
                if r[d]==0:p+=n if n<128 else n-256
        elif op==7:r[b1[p]&7]=length;p+=1
        elif op==12:r[0]|=vm2(r[4])
        else:raise ValueError(("vm1",hex(at),op))
        state=(33*state+7+op)&255

regs=vm1()
check(regs[0],"final error")
print("Constraints:",len(solver.assertions()))
print(solver.check())
if solver.check()==z3.sat:
    m=solver.model(); n=m.eval(length).as_long()
    answer=bytes(m.eval(x).as_long() for x in password[:n])
    print("PASSWORD:",answer.decode())
    Path("password.txt").write_bytes(answer+b"\n")
    solver.add(z3.Or([x!=m.eval(x) for x in password[:n]]))
    print("Alternative password:",solver.check())
