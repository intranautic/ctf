# C++ Object Layout

Notes for the layout of c++ objects in memory.

When defining classes, fields are typically ordered in the order of declaration.

Classes dont actually contain method pointers if they are defined statically (disregarding dynamic dispatch). Below is a simple example.

```cpp
struct A {
  int x;
  int y;
  int f() const { return x + y; }
};
```

The object in memory will be shown down below, disregarding various allocation primitives which may affect the initial values of the chunks when allocated either statically or dynamically.

Just as an example, lets assume our allocator zeroes out all memory when allocated, or that the stack doesnt contain any arbitrary values.

```cpp
0x0: 0x00000000 // (int)A::x;
0x4: 0x00000000 // (int)A::y;
```

When we actually call `A::f()`, it is linked as a static function and called the same as any other function with some discrepancies in calling convention and arguments. The general signature is defined below (disregarding name mangling at compile time).

```cpp
int __thiscall A::f(A* this) const;
```

When an derived class inherits from a base class, it's fields are emplaced after the base class's field within memory.

```cpp
struct B: A {int z;};
```

```
0x0: 0x00000000 // (int)A::x;
0x4: 0x00000000 // (int)A::y;
0x8: 0x00000000 // (int)B::z;
```

Virtual methods are a means of achieving dynamic dispatch, also known as runtime polymorphism.

This allows for various runtime features to the language including inheritance hierarchies which allow derived classes to implement specialized behavior whilst adhering to the common interface of the base class.

When defining a class which is designed to be inherited from by a derived class, you need to define Virtual Destructors.

The standard spec doesnt actually require the existence of virtual method tables or any standard implementation of dynamic dispatch, however all compiler infrastructures ive seen implement virtual tables.

There is one vtable per class, meaning that all instances of objects of the same class share the same vtable. Objects which belong to the same class heirarchy will have different vtables with the same layout meaning, each methods offset will correspond with each other.

Each object instance contains a hidden field to the virtual table. The virtual table contains the addresses of all of the classes virtual methods.

Consider the code shown as follows.

```cpp
class B1 {
public:
  void fnonvirtual() {}
  virtual void f1() {}
  int int_in_b1;
};

class B2 {
public:
  virtual void f2() {}
  int int_in_b2;
};

class D : public B1, public B2 {
public:
  void d() {}
  void f2() override {}
  int int_in_d;
};

int main(void) {
  B2 *b2 = new B2();
  D  *d  = new D();
  return 0;
}
```

For `gcc`, the vtable layout is as follows.

```
b2:
  +0: pointer to virtual method table of B2
  +4: value of int_in_b2

virtual method table of B2:
  +0: B2::f2()


d:
  +0: pointer to virtual method table of D (for B1)
  +4: value of int_in_b1
  +8: pointer to virtual method table of D (for B2)
 +12: value of int_in_b2
 +16: value of int_in_d

virtual method table of D (for B1):
  +0: B1::f1()  // B1::f1() is not overridden

virtual method table of D (for B2):
  +0: D::f2()   // B2::f2() is overridden by D::f2()
```

Since `gcc` implements multiple inheritance using one vtable per inherited base class, there is a necessity for whats known as pointer fixups in the form of thunks.

Below is an example which demonstrates this using the previous class definitions.

```cpp
D  *d  = new D();
B1 *b1 = d; // d + 0
B2 *b2 = d; // d + 8
```

We can see that the second cast when assigning `d` to `b2`, it will fix the pointer to the region of `d` which resembles the b2 object.

In the case of pure virtual functions, the compiler will allocate a slot on the vtable for it but it will have no assigned address.

For some fucking reason there are two virtual destructors??

```cpp
0000000000210990 ; `vtable for'Ghost
0000000000210990 _ZTV5Ghost      dq 0
0000000000210998 dq offset _ZTI5Ghost ; typeinfo
00000000002109A0 dq offset Ghost::speak
00000000002109A8 dq offset Ghost::changemsg
00000000002109B0 dq offset Ghost::ghostinfo
00000000002109B8 dq offset Ghost::~Ghost
00000000002109C0 dq offset Ghost::~Ghost
```

Decompilation for both are similar with one variance shown below.

```c
void __fastcall Ghost::~Ghost1(void *this)
{
  size_t v2; // rax
  void *v3; // rdi
  char *v4; // rdi
  char *v5; // rdi

  *(_QWORD *)this = &`vtable for'Ghost + 2;
  *((_DWORD *)this + 2) = 0;
  *((_QWORD *)this + 8) = 0LL;
  **((_BYTE **)this + 7) = 0;
  *((_QWORD *)this + 4) = 0LL;
  **((_BYTE **)this + 3) = 0;
  v2 = malloc_usable_size(*((void **)this + 2));
  memset(*((void **)this + 2), 0, v2);
  v3 = (void *)*((_QWORD *)this + 2);
  if ( v3 )
    operator delete[](v3);
  v4 = (char *)*((_QWORD *)this + 7);
  if ( v4 != (char *)this + 72 )
    operator delete(v4);
  v5 = (char *)*((_QWORD *)this + 3);
  if ( v5 != (char *)this + 40 )
    operator delete(v5);
  operator delete(this);
}

void __fastcall Ghost::~Ghost2(void *this)
{
  size_t v2; // rax
  void *v3; // rdi
  char *v4; // rdi
  char *v5; // rdi

  *(_QWORD *)this = &`vtable for'Ghost + 2;
  *((_DWORD *)this + 2) = 0;
  *((_QWORD *)this + 8) = 0LL;
  **((_BYTE **)this + 7) = 0;
  *((_QWORD *)this + 4) = 0LL;
  **((_BYTE **)this + 3) = 0;
  v2 = malloc_usable_size(*((void **)this + 2));
  memset(*((void **)this + 2), 0, v2);
  v3 = (void *)*((_QWORD *)this + 2);
  if ( v3 )
    operator delete[](v3);
  v4 = (char *)*((_QWORD *)this + 7);
  if ( v4 != (char *)this + 72 )
    operator delete(v4);
  v5 = (char *)*((_QWORD *)this + 3);
  if ( v5 != (char *)this + 40 )
    operator delete(v5);
}
```

`Ghost::~Ghost2` doesnt actually free the object pointed to by the `this` pointer. Well why the fuck not??

Ohh wait im a fucking idiot, one is the base object destructor and the other is the delete destructor, thanks https://reverseengineering.stackexchange.com/questions/4323/why-two-virtual-destructors.

When the object is allocated on the stack there is no need to actually explicitly deallocate the frame will simply be deallocated. But for dynamically allocated objects you have to explicitly do so, i thought the `delete` keyword managed that..?

I was under the impression that delete would call the destructor then maybe inline or introduce a thunk which would deallocate the pointer? strange decision to make two destructors.. Is this only a thing for virtual destructors??

Maybe thats why?? Maybe its specific to virtual destructors?o
