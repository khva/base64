# base64 - tiny C++ encoding & decoding library (header-only)

[![C++](https://img.shields.io/badge/c%2B%2B-20-informational.svg)](https://shields.io/)
[![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/)
![](https://github.com/khva/kvstor/workflows/linux/badge.svg)
![](https://github.com/khva/kvstor/workflows/macos/badge.svg)
![](https://github.com/khva/kvstor/workflows/windows/badge.svg)


## Table of Contents
- [Description](#description)
- [How to use the library](#how-to-use-the-library)
  - [Quick start](#quick-start)
  - [Base64 encoding](#base64-encoding)
  - [Base64 decoding](#base64-decoding)
  - [Error handling](#error-handling)
- [How to add library to your project](#how-to-add-library-to-your-project)
- [Used third-party tools](#used-third-party-tools)


## Description
The base64 library implements Base64 encoding and decoding functions.

Its main features:
 - header-only and easy to use
 - supports different types of STL containers: `std::string`, `std::vector`, `std::string_view`, `std::array`
 - easily adaptable to use custom containers/buffers
 - supports standard and URL alphabets
 - easily adaptable to use custom Base64 alphabet
 - cross-platform (tested on GCC, Clang and MSVC)
 - no external dependencies (only the doctest library is used as part of the test subproject)


## How to use the library
The top-level interface functions are defined in the header file `base64/base64.h`.

### Quick start
Example of encoding data in Base64:
```c++
constexpr std::string_view data = "0123456789";

// calculate the size of output buffer
const size_t encoded_size = base64::calc_encoded_size(data.size());
std::string encoded;

// allocate a buffer of sufficient size
encoded.resize(encoded_size);

// call the encoding function
base64::encode(data, encoded);
assert(encoded == "MDEyMzQ1Njc4OQ==");

```

Example of decoding data from Base64:
```c++
constexpr std::string_view data = "MDEyMzQ1Njc4OQ==";

// calculate the size of output buffer
const size_t decoded_size = base64::calc_decoded_size(data);
std::string decoded;

// allocate a buffer of sufficient size
decoded.resize(decoded_size);

// call the decoding function
base64::decode(data, decoded);
assert(decoded == "0123456789");

```


### Base64 encoding
There are two encoding functions in `base64.h`:
```c++
template <typename raw_array, typename base64_array>
error_code_t encode(const raw_array & raw_data, base64_array & base64_data);

template <typename raw_array, typename base64_array>
error_code_t encode_url(const raw_array & raw_data, base64_array & base64_data);
```
Both functions encode incoming data into Base64 and put the result into the output buffer. The difference between these functions lies in the alphabets used. The `encode()` function use standard Base64 alphabet and the `encode_url()` use ["URL and Filename Safe"](https://www.rfc-editor.org/rfc/rfc3548) Base64 alphabet.

**Parameters:**
 - `raw_data` — the input buffer containing data to encode, supported buffer types: [std::basic_string](https://en.cppreference.com/w/cpp/string/basic_string), [std::vector](https://en.cppreference.com/w/cpp/container/vector), [std::basic_string_view](https://en.cppreference.com/w/cpp/string/basic_string_view), [std::array](https://en.cppreference.com/w/cpp/container/array)
 - `base64_data` — the output buffer containing encoded data after successful execution, supported buffer types: [std::basic_string](https://en.cppreference.com/w/cpp/string/basic_string), [std::vector](https://en.cppreference.com/w/cpp/container/vector), [std::array](https://en.cppreference.com/w/cpp/container/array)

Encoding functions return an error code of type `error_code_t`. The `error_code_t::no_error()` method returns `true` if encoding is successful. Possible error types:
 - `error_type_t::no_error` — no error
 - `error_type_t::insufficient_buffer_size` — insufficient size of output buffer

For more information about errors, see the [Error handling](#error-handling) section.

**Important:** encoding functions do not allocate any dynamic memory, so you need to allocate a buffer of sufficient size before encoding. To do this, use the encoded size calculation functions:

```c++
size_t calc_encoded_size(size_t raw_size);
size_t calc_encoded_size_url(size_t raw_size);
```
Both functions calculate the encoding buffer size based on the size of the input data. Similar to encoding functions, the first function uses the standard Base64 alphabet, and the second uses the "URL and Filename Safe" Base64 alphabet.

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

    constexpr std::string_view video_attr = R"({"is_full_screen":false,"window_size":{"width":400,"height":200}})";
    std::string video_base64(calc_encoded_size_url(video_attr.size()), '\0');
    encode_url(video_attr, video_base64);

    std::cout << "original video attributes: " << video_attr << std::endl;
    std::cout << "base64 video attributes:   " << video_base64 << std::endl;
}
```
Expected output:
```
original video attributes: {"is_full_screen":false,"window_size":{"width":400,"height":200}}
base64 video attributes:   eyJpc19mdWxsX3NjcmVlbiI6ZmFsc2UsIndpbmRvd19zaXplIjp7IndpZHRoIjo0MDAsImhlaWdodCI6MjAwfX0
```


### Base64 decoding
There are two decoding functions in _"base64.h"_:
```c++
template <typename base64_array, typename raw_array>
error_code_t decode(const base64_array & base64_data, raw_array & raw_data);

template <typename base64_array, typename raw_array>
error_code_t decode_url(const base64_array & base64_data, raw_array & raw_data);
```
Both functions decode incoming data from Base64 and put the result into the output buffer. The difference between these functions lies in the alphabets used. The `decode()` function use standard Base64 alphabet and the `decode_url()` use ["URL and Filename Safe"](https://www.rfc-editor.org/rfc/rfc3548) Base64 alphabet.

**Parameters:**
 - `base64_data` — the input buffer containing encoded data, supported buffer types: [std::basic_string](https://en.cppreference.com/w/cpp/string/basic_string), [std::vector](https://en.cppreference.com/w/cpp/container/vector), [std::basic_string_view](https://en.cppreference.com/w/cpp/string/basic_string_view), [std::array](https://en.cppreference.com/w/cpp/container/array)
 - `raw_data` — the output buffer containing decoded data after successful execution, supported buffer types: [std::basic_string](https://en.cppreference.com/w/cpp/string/basic_string), [std::vector](https://en.cppreference.com/w/cpp/container/vector), [std::array](https://en.cppreference.com/w/cpp/container/array)

Encoding functions return an error code of type `error_code_t`. The `error_code_t::no_error()` method returns `true` if encoding is successful. Possible error types:
 - `error_type_t::no_error` — no error
 - `error_type_t::insufficient_buffer_size` — insufficient size of output buffer
 - `error_type_t::invalid_buffer_size` — invalid size of input buffer, the buffer is truncated or corrupted
 - `error_type_t::non_alphabetic_symbol` — the input buffer contains a non-alphabetic symbol

For more information about errors, see the [Error handling](#error-handling) section.

**Important:** decoding functions do not allocate any dynamic memory, so you need to allocate a buffer of sufficient size before decoding. To do this, use the decoded size calculation functions:
```c++
template <typename base64_array>
size_t calc_decoded_size(const base64_array & base64_data);

template <typename base64_array>
size_t calc_decoded_size_url(const base64_array & base64_data);
```
Both functions calculate the decoding buffer size based on the encoded data. Similar to decoding functions, the first function uses the standard Base64 alphabet, and the second uses the "URL and Filename Safe" Base64 alphabet.

**Parameters:**
 - `base64_data` — the input buffer containing data to decode

The calculation functions never fail.

#### Example 2: Base64 decoding
```c++
#include "base64.h"
#include <iostream>

void base64_decoding()
{
    using namespace base64;

    constexpr std::string_view video_base64 = "eyJpc19mdWxsX3NjcmVlbiI6ZmFsc2UsIndpbmRvd19zaXplIjp7IndpZHRoIjo0MDAsImhlaWdodCI6MjAwfX0";
    std::string video_attr(calc_decoded_size_url(video_base64), '\0');
    decode_url(video_base64, video_attr);

    std::cout << "base64 video attributes:  " << video_base64 << std::endl;
    std::cout << "decoded video attributes: " << video_attr << std::endl;
}
```
Expected output:
```
base64 video attributes:  eyJpc19mdWxsX3NjcmVlbiI6ZmFsc2UsIndpbmRvd19zaXplIjp7IndpZHRoIjo0MDAsImhlaWdodCI6MjAwfX0
decoded video attributes: {"is_full_screen":false,"window_size":{"width":400,"height":200}}
```


### Error handling
TODO: add description

