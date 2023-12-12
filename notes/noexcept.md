# noexcept

c++ pwn challenge concept

no name yet, just a potential primitive?

custom container with a template move constructor not marked as noexcept which allows the function to potentially throw. it will be used to contain an object which can throw an exception.

while the move constructor is executing if an exception is thrown the rest of the operation wont be completed and both the src and dest objects will be in an invalid state.

take the following example into consideration.

```cpp
struct A {
  void* x;
  void* y;
};

A::A(A&& _rhs) {
  this->x = _rhs.x;
  this->y = _rhs.y;
  // exception potentially thrown
  _rhs.x = _rhs.y = nullptr;
}
```

the move constructor should transfer ownership of the object, but when the exception is thrown it returns, unwinds the call stack and never clears the dangling reference. Now if the objects `this` and `_rhs` both have destructors which free the resource.

we could get a double free or a ptr to a free chunk with this and uaf bug. can also get more creative, but this is a pretty interesting concept, could be used for some interesting challs?

lol c++ is littered with implicit operations and rules, so many things to abuse. complexity is the tradeoff for abstraction power i guess?
