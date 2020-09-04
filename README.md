C++98 string class that provides functionality of both `std::string` and `std::string_view`. 

It was inteded for boost library, but lacks documentation.

This allows functions to always take `boost::const_string` argument without memory allocations, unlike `std::string`. And no need for separate overloads that take `std::string_view` instead of `std::string`.

Small string optimization is there and the small string buffer size is controlled by the template argument. 

Longer strings are reference-counted, so that copies are cheap.

The class is called `const_string` because it doesn't allow modification of individual characters of the string. And that allows for reference counting with no undefined behaviour.

Examples:

```
 / No memory allocation due to small string optimization.
const_string a("abc");

// Memory allocation because the string is long.
const_string b("abcdefghijklmnoprstqvwxyz"); 

// No memory allocation, reference counter increment.
const_string b2 = b; 

// No memory allocation, the string literal is referred to.
// Equivalent to using std::string_view.
const_string c(boost::cref("abcdefghijklmnoprstqvwxyz")); 
```

TODO: Update with unit tests and C++11 `std::` classes.
