# interface
`interface` does the exact same thing as go's interface in c++.
It's syntax is

````c++
INTERFACE(signature 0, method name 0, signature 1, method name 1, ...)
````

which is an anonymous type which holds anything with methods matching the specified name and sufficiently similar signature.
The rules of similarity follows that of `std::function`.

`using I = INTERFACE(sig0, id0, sig1, id1, ...)` is equivalent to the go construct

````go
type I interface {
  id0 sig0
  id1 sig1
  ...
}
````

## Installation

`interface` is a header only library. Just `#include "interface.hpp"`.

## General constraints

`interface` methods may not be overloaded.

Can be defined at namespace and class scope, but not at function scope.

Must have at least one method. Use `std::any` instead for empty interfaces.

Pointers to objects give `interface` reference semantics.

Requires C++17.

Uses a lot of macros. If you hate that, support and help standardize Herb Sutter's metaclasses.

Has a default maximum of 8 methods in the interface. See impl/README for details.

## Example 1

````c++
using Fooer = INTERFACE(void(), foo);
struct R {
  void foo() {}
};
struct S {
  void foo() {} 
  void bar(int) {}
};

void foo()
{
  Fooer i = R{};
  i.foo();
  i = S{};
  i.foo();
}
````

The `i.foo()` calls will call the underlying object's `foo()`

## Example 2

````c++
using Foobarer = INTERFACE(void(), foo, void(int), bar);
struct S {
  void foo() {}
  void bar(int) {}
};

void foobar()
{
  Foobarer f = S{};
  f.foo();
  f.bar(42);
}
````

`interface` supports multiple methods.

## Example 3

````c++
using interface = INTERFACE(void(), answer);
struct S {
  int n = 0;
  void answer() { n = 42; }
};

void meaning()
{
  S s;
  interface i = s;
  i.answer();
  assert(s.n == 0);
  i = &s;
  i.answer();
  assert(s.n == 42);
}
````

Pointers gives `interface` reference semantics.

## Example 4

````c++
INTERFACE(std::pair<int, float>(), fails);

using signature = std::pair<int, float>();
INTERFACE(signature, works);
````

The alias is necessary since macros don't respect angle brackets.

````c++
INTERFACE(void(std::pair<int, float>), also_works);
````

An alias isn't needed here because it's within brackets.

## Example 5

````c++
INTERFACE(interface(interface, interface), works);
INTERFACE(void(std::vector<interface>), still_works);
INTERFACE(interface&&(*(std::vector<interface>&, interface*(*)[10]))(), this_is_fine);
````

`interface` refers to the type itself within its definition.

## Example 6

````c++
using bad_signature = void(std::map<string, interface>);
INTERFACE(bad_signature, fails);

template<typename T>
using good_signature = void(std::map<string, T>);
INTERFACE(good_signature<interface>, works);
````

`interface` isn't visible outside its definition.

## Example 7

````c++
template<typename... Ts>
struct S {
  using type = INTERFACE(void(Ts&&...), just_works);
};
````

Works with templates as well.

## Other functions

#### `interface(T&& t)`
Constructs an interface from `t` that have methods similar to interface methods. Similarity follows that of `std::function`.

#### `friend T* target<T>(interface&& i)`
#### `friend T* target<T>(interface& i)`
#### `friend const T* target<T>(const interface& i)`
Returns a pointer to the underlying object. Returns `nullptr` if type doesn't match.
A declaration of `target` must be visible at the call site. `using adl_helper::target;` can be used.

#### `operator bool()`
Tests whether the interface holds anything.

#### `friend swap(interface& x, interface& y)`
Swaps the contents of the interfaces.

All other special member functions all behave like they should.
There is no conversion between different interfaces unless one is a subset of another.

## Well-definedness

Invokes no undefined behaviour that I am aware of (including arcane pointer rules).

## Anonymous type

Actually, the type is a name appended with the line number. It is therefore advised to avoid defining `INTERFACE` in different translation units in the same namespace to avoid odr violations.

