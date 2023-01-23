# base64 - tiny C++ encoding & decoding library (header-only)

[![C++](https://img.shields.io/badge/c%2B%2B-20-informational.svg)](https://shields.io/)
[![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/)
![](https://github.com/khva/kvstor/workflows/linux/badge.svg)
![](https://github.com/khva/kvstor/workflows/macos/badge.svg)
![](https://github.com/khva/kvstor/workflows/windows/badge.svg)


## Table of Contents
- [Description](#description)
- [How to use library](#how-to-use-library)
  - [Quick start](#quick-start)
  - [Base64 encoding](#base64-encoding)
  - [Base64 decoding](#base64-decoding)
  - [Error handling](#error-handling)
- [How to add library to your project](#how-to-add-library-to-your-project)
- [Used third-party tools](#used-third-party-tools)


## Description
The base64 library implements Base64 encoding and decoding functions.

Its main features:
 - support different types of STL containers: `std::string`, `std::vector`, `std::string_view`, `std::array`
 - easily adaptable to use custom containers/buffers
 - support standard and URL alphabets
 - easily adaptable to use custom Base64 alphabet
 - header-only
 - cross-platform (tested on GCC, Clang and MSVC)
 - no external dependencies (only the doctest library is used as part of the test subproject)


## How to use library
The top-level interface functions are defined in the header file _"base64/base64.h"_.

### Quick start
To encode data in Base64 you need:
 1. calculate the size of output buffer
 2. allocate a buffer of sufficient size
 3. call the encoding function

```c++
const std::string_view data = "0123456789";
const size_t encoded_size = base64::calc_encoded_size(data.size());  // 1.

std::string encoded;
encoded.resize(encoded_size);  // 2.

base64::encode(data, encoded);  // 3.
assert(encoded == "MDEyMzQ1Njc4OQ==");

```

To decode data from Base64 you need:
 1. calculate the size of output buffer
 2. allocate a buffer of sufficient size
 3. call the decoding function

```c++
const std::string_view data = "MDEyMzQ1Njc4OQ==";
const size_t decoded_size = base64::calc_decoded_size(data);  // 1.

std::string decoded;
decoded.resize(decoded_size);  // 2.

base64::decode(data, decoded);  // 3.
assert(decoded == "0123456789");

```


### Base64 encoding
There are two encoding functions in _"base64.h"_:
```c++
template <typename raw_array, typename base64_array>
error_code_t encode(const raw_array & raw_data, base64_array & base64_data);

template <typename raw_array, typename base64_array>
error_code_t encode_url(const raw_array & raw_data, base64_array & base64_data);
```
Both functions encode incoming data into Base64 and put the result into the output buffer. The difference between these functions lies in the alphabets used. The `encode()` function use standard alphabet and the `encode_url()` use ["URL and Filename Safe"](https://www.rfc-editor.org/rfc/rfc3548) alphabet.

**Parameters:**
 - `raw_data` — the input buffer containing data to encode, supported data types: [std::basic_string](https://en.cppreference.com/w/cpp/string/basic_string), [std::vector](https://en.cppreference.com/w/cpp/container/vector), [std::basic_string_view](https://en.cppreference.com/w/cpp/string/basic_string_view), [std::array](https://en.cppreference.com/w/cpp/container/array)
 - `base64_data` — the output buffer containing encoded data after successful execution, supported data types: [std::basic_string](https://en.cppreference.com/w/cpp/string/basic_string), [std::vector](https://en.cppreference.com/w/cpp/container/vector), [std::array](https://en.cppreference.com/w/cpp/container/array)

Encoding functions return an error code of type `error_code_t`. The `error_code_t::no_error()` method returns `true` if encoding is successful. Possible error types:
 - `error_type_t::no_error` — no error
 - `error_type_t::insufficient_buffer_size` — insufficient size of output buffer

For more information about errors, see the [Error handling](#error-handling) section.

**Important:** Encoding functions do not allocate any dynamic memory, so you need to allocate a buffer of sufficient size before encoding. To do this, use the output buffer size calculation functions:

```c++
size_t calc_encoded_size(size_t raw_size);
size_t calc_encoded_size_url(size_t raw_size);
```
Both functions calculate the size of the encoding buffer based on the size of the input data. Similar to encoding functions, the first function uses the standard alphabet, and the second uses the ["URL and Filename Safe"](https://www.rfc-editor.org/rfc/rfc3548) alphabet.

**Parameters:**
 - `raw_size` — the size of input buffer containing data to encode

The calculation functions never fail.

#### Example 1: Base64 encoding
```c++
#include "base64.h"
#include <iostream>

void base64_encoding()
{
    using namespace base64;

    constexpr std::string_view video_attr = R"({"is_full_sreen":false,"window_size":{"width":400,"height":200}})";
    std::string video_base64(calc_encoded_size_url(video_attr.size()), '\0');
    encode_url(video_attr, video_base64);

    std::cout << "original video attributes: " << video_attr << std::endl;
    std::cout << "base64 video attributes:   " << video_base64 << std::endl;
}
```
Expected output:
```
original video attributes: {"is_full_sreen":false,"window_size":{"width":400,"height":200}}
base64 video attributes:   eyJpc19mdWxsX3NyZWVuIjpmYWxzZSwid2luZG93X3NpemUiOnsid2lkdGgiOjQwMCwiaGVpZ2h0IjoyMDB9fQ
```


### Base64 decoding
There are two decoding functions in _"base64.h"_:
TODO: add description


### Error handling
TODO: add description

