# Overview 
C++98 immutable string class that provides functionality of both `std::string` and `std::string_view`. 

The class is called `const_string` because it doesn't allow modification of individual characters of the string. This restriction allows for cheap copying with thread-safe reference counting. The library is inteded for inclusion into boost library, but lacks documentation required for library acceptance review.

GNU C++98 `std::string` was reference-counted, but a C++ standard compliant `std::string` cannot be both reference-counted and fully standard compliant, so that GNU C++11 `std::string` is no longer reference-counted. The mutability of individual characters of `std::string` is the main reason it cannot be reference-counted. `const_string` ditches mutability of individual characters in order to provide a cheap to copy string class, also with `std::string_view` functionality.

The purpose of `const_string` is to allow functions to always take `const_string` argument and:

* Not require memory allocation, unlike functions taking `std::string` argument.
* Not require a separate overload function for `std::string_view` to avoid `std::string` memory allocation.
* Have a customizable small string buffer size. The minimum size of a `const_string` is 16 bytes, which allows for 8-byte small string optimization on a 64-bit platform. 

Small string optimization is implemented. The small string buffer size is controlled by the template argument, if you need to change the default.

Longer strings are reference-counted, so that copies are cheap and thread-safe. 

# Examples

```
using boost::const_string;

// No memory allocation due to small string optimization.
const_string a("abc");

// Memory allocation because the string is long.
const_string b0("abcdefghijklmnoprstqvwxyz"); 

// No memory allocation, reference counter increment.
const_string b1 = b0; 

// No memory allocation, the string literal is referred to.
// Equivalent to using std::string_view.
const_string c(boost::cref("abcdefghijklmnoprstqvwxyz")); 

// Refer to std::string with no memory allocation or copy.
// Equivalent to using std::string_view.
std::string d0("abcdefghijklmnoprstqvwxyz");
const_string d1(boost::cref(d0)); 
```

# TODO

* Documentation.
* Update with unit tests and C++11 `std::` classes.
* Use Facebook small string optimization to allow for 7 more characters.
