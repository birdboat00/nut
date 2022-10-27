# nut

A C++ header-only library of various personal utilities and types I use.

## Prerequisites
- A C++20 compatible C++ compiler
- CMake (to build the tests)

## Compilers
The library code and test code is known to function with:
- msvc19

## Installation
As this is a header-only library all you have to do is to add the `include` subdirectory to your compilers include path.

### Using CMake
If your project uses CMake the library can be included as a subdiretory.
Alternatively you can use `FetchContent` which should work too, but is not yet tested.

## Components and Functionality
The library contains a bunch of different features:
- Type definitions for basic data types
  - `u8, u8, u16, i16, u32, i32, u64, i64` for integer data types by bits
  - `f32 and f64` as floating point number data types
  - `str (wstr) and strview (wstrview)` as alternative names to `std::string (std::wstring) and std::string_view (std::wstring_view)`
  - `env::args() and env::init_args(argc, argv)` as a global program arguments container
  - `size_of<T>` as alternative to `sizeof(T)`
  - `array_byte_size(array)` to get the byte size in memory of the contents of an `std::array`
  - `toggle_bool(bool)` to toggle a boolean reference
  - `print(fmt, args), println(fmt, args)` to print formatted output almost like rusts `println!` macros.
  - `panic(fmt, args) todo() unreachable() unimplemented()` functions
  - `struct id` for identifiers (namespace:path/to/thing) or tags (#namespace:type/of/tag)

## License
nut is licensed under the [MIT license](LICENSE).