# Overview 
C++98 immutable string class that provides functionality of both `std::string` and `std::string_view`. 

The main purpose of `const_string` is to allow functions that take/return immutable strings (most of functions) to use only `const_string` for all kinds of strings and not require memory allocation.

The class is called `const_string` because it doesn't allow modification of individual characters of the string. This restriction allows for cheap copying with thread-safe reference counting. The library is inteded for inclusion into boost library, but lacks documentation required for library acceptance review.

GNU C++98 `std::string` was reference-counted, but a C++ standard compliant `std::string` cannot be both reference-counted and fully standard compliant, so that GNU C++11 `std::string` is no longer reference-counted. The mutability of individual characters of `std::string` is the main reason it cannot be reference-counted. 

`const_string` doesn't allow modification of individual characters in order to provide cheap/shallow copies and also provides `std::string_view` functionality to allow for no memory allocation usage.

## Main features

* Optional memory allocation, unlike functions taking `std::string` argument.
* No extra overloads for `std::string_view` to avoid `std::string` memory allocation.
* Customizable small string optimization buffer size. On a 64-bit platform, the minimum size of a `const_string` is 16 bytes, which allows for 8-byte small string optimization. The small string buffer size is controlled by the template argument, should you need to change the default.
* Longer strings are reference-counted, so that copies are cheap and thread-safe. 

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
* Update with C++11.
* Use Facebook small string optimization to allow for 7 more characters with no memory allocation.
