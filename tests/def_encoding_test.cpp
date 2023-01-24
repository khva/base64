#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <string>
#include <string_view>
#include <vector>

#include "doctest/doctest.h"
#include "base64.h"
#include "helpers.h"

void non_alphabetical_character()
{
    using namespace base64;

    // the non-alphabetical character '(' at index 3
    constexpr std::string_view wrong_data = "MDE(MzQ1Njc4OUFC";

    std::string decoded(calc_decoded_size(wrong_data), '\0');
    const error_code_t code = decode(wrong_data, decoded);

    if (!code)
    {
        std::cout << "An error has occurred. " << code.msg() << std::endl;
        return;
    }
    std::cout << "decoded data: " << decoded << std::endl;
}


TEST_CASE("encode_errors")
{
    non_alphabetical_character();
    using namespace base64;

    const std::string_view data = "0123456789AB";
    std::string encoded;
    const error_code_t code = encode(data, encoded);

    REQUIRE(!code);
    REQUIRE(code.type() == error_type_t::insufficient_buffer_size);
    REQUIRE(code.msg() == "The buffer has insufficient size (required - 16, obtained - 0).");
    REQUIRE(encoded.empty());
}


TEST_CASE("decode_errors")
{
    using namespace base64;

    const std::string_view data = "MDEyMzQ1Njc4OUFC";
    const std::string_view bad_buffer_1 = "MDEyMzQ1Njc4OUF";
    const std::string_view bad_buffer_2 = "MDEyMzQ1Njc4OU";
    const std::string_view bad_buffer_3 = "MDEyMzQ1Njc4O";
    const std::string_view bad_symbol = "MDEyMzQ1Nj*4OUFC";

    std::string decoded;
    error_code_t code = decode(data, decoded);

    REQUIRE(!code);
    REQUIRE(code.type() == error_type_t::insufficient_buffer_size);
    REQUIRE(code.msg() == "The buffer has insufficient size (required - 12, obtained - 0).");
    REQUIRE(decoded.empty());

    const size_t decoded_size = calc_decoded_size(data);
    decoded.resize(decoded_size);

    code = decode(bad_buffer_1, decoded);
    REQUIRE(code.type() == error_type_t::invalid_buffer_size);
    REQUIRE(code.msg() == "The base64 buffer has invalid size of 15. The buffer size must be a multiple of 4.");

    code = decode(bad_buffer_2, decoded);
    REQUIRE(code.type() == error_type_t::invalid_buffer_size);
    REQUIRE(code.msg() == "The base64 buffer has invalid size of 14. The buffer size must be a multiple of 4.");

    code = decode(bad_buffer_3, decoded);
    REQUIRE(code.type() == error_type_t::invalid_buffer_size);
    REQUIRE(code.msg() == "The base64 buffer has invalid size of 13. The buffer size must be a multiple of 4.");

    code = decode(bad_symbol, decoded);
    REQUIRE(code.type() == error_type_t::non_alphabetic_symbol);
    REQUIRE(code.msg() == "The buffer has the non-alphabetical character 0x2A at index 10.");
}


TEST_CASE("encode_empty_data")
{
    using namespace base64;

    const std::string_view data;
    constexpr size_t expected_size = 0;
    const size_t encoded_size = calc_encoded_size(data.size());
    REQUIRE(encoded_size == expected_size);

    std::string encoded;
    const error_code_t code = encode(data, encoded);
    REQUIRE(code);
    REQUIRE(encoded.empty());
}


TEST_CASE("decode_empty_data")
{
    using namespace base64;

    const std::string_view data;
    constexpr size_t expected_size = 0;
    const size_t decoded_size = calc_decoded_size(data);
    REQUIRE(decoded_size == expected_size);

    std::string decoded;
    const error_code_t code = decode(data, decoded);
    REQUIRE(code);
    REQUIRE(decoded.empty());
}


TEST_CASE("encode_with_tails_012")
{
    using namespace base64;

    const std::string_view data_1 = "0";
    const std::string_view data_2 = "01";
    const std::string_view data_3 = "012";
    const std::string_view data_10 = "0123456789";
    const std::string_view data_11 = "0123456789A";
    const std::string_view data_12 = "0123456789AB";

    constexpr size_t expected_size_1_2_3 = 4;
    constexpr size_t expected_size_10_11_12 = 16;

    constexpr std::string_view expected_result_1 = "MA==";
    constexpr std::string_view expected_result_2 = "MDE=";
    constexpr std::string_view expected_result_3 = "MDEy";
    constexpr std::string_view expected_result_10 = "MDEyMzQ1Njc4OQ==";
    constexpr std::string_view expected_result_11 = "MDEyMzQ1Njc4OUE=";
    constexpr std::string_view expected_result_12 = "MDEyMzQ1Njc4OUFC";

    const size_t encoded_size_1 = calc_encoded_size(data_1.size());
    REQUIRE(encoded_size_1 == expected_size_1_2_3);

    const size_t encoded_size_2 = calc_encoded_size(data_2.size());
    REQUIRE(encoded_size_2 == expected_size_1_2_3);

    const size_t encoded_size_3 = calc_encoded_size(data_3.size());
    REQUIRE(encoded_size_3 == expected_size_1_2_3);

    const size_t encoded_size_10 = calc_encoded_size(data_10.size());
    REQUIRE(encoded_size_10 == expected_size_10_11_12);

    const size_t encoded_size_11 = calc_encoded_size(data_11.size());
    REQUIRE(encoded_size_11 == expected_size_10_11_12);

    const size_t encoded_size_12 = calc_encoded_size(data_12.size());
    REQUIRE(encoded_size_12 == expected_size_10_11_12);

    std::string encoded;
    encoded.resize(expected_size_1_2_3);
    error_code_t code = encode(data_1, encoded);
    REQUIRE(code);
    REQUIRE(encoded == expected_result_1);

    encoded.clear();
    encoded.resize(expected_size_1_2_3);
    code = encode(data_2, encoded);
    REQUIRE(code);
    REQUIRE(encoded == expected_result_2);

    encoded.clear();
    encoded.resize(expected_size_1_2_3);
    code = encode(data_3, encoded);
    REQUIRE(code);
    REQUIRE(encoded == expected_result_3);

    encoded.clear();
    encoded.resize(expected_size_10_11_12);
    code = encode(data_10, encoded);
    REQUIRE(code);
    REQUIRE(encoded == expected_result_10);

    encoded.clear();
    encoded.resize(expected_size_10_11_12);
    code = encode(data_11, encoded);
    REQUIRE(code);
    REQUIRE(encoded == expected_result_11);

    encoded.clear();
    encoded.resize(expected_size_10_11_12);
    code = encode(data_12, encoded);
    REQUIRE(code);
    REQUIRE(encoded == expected_result_12);
}


TEST_CASE("decode_with_tails_012")
{
    using namespace base64;

    constexpr std::string_view encoded_1 = "MA==";
    constexpr std::string_view encoded_2 = "MDE=";
    constexpr std::string_view encoded_3 = "MDEy";
    constexpr std::string_view encoded_10 = "MDEyMzQ1Njc4OQ==";
    constexpr std::string_view encoded_11 = "MDEyMzQ1Njc4OUE=";
    constexpr std::string_view encoded_12 = "MDEyMzQ1Njc4OUFC";

    constexpr size_t expected_size_1 = 1;
    constexpr size_t expected_size_2 = 2;
    constexpr size_t expected_size_3 = 3;
    constexpr size_t expected_size_10 = 10;
    constexpr size_t expected_size_11 = 11;
    constexpr size_t expected_size_12 = 12;

    const std::string_view expected_result_1 = "0";
    const std::string_view expected_result_2 = "01";
    const std::string_view expected_result_3 = "012";
    const std::string_view expected_result_10 = "0123456789";
    const std::string_view expected_result_11 = "0123456789A";
    const std::string_view expected_result_12 = "0123456789AB";

    const size_t decoded_size_1 = calc_decoded_size(encoded_1);
    REQUIRE(decoded_size_1 == expected_size_1);

    const size_t decoded_size_2 = calc_decoded_size(encoded_2);
    REQUIRE(decoded_size_2 == expected_size_2);

    const size_t decoded_size_3 = calc_decoded_size(encoded_3);
    REQUIRE(decoded_size_3 == expected_size_3);

    const size_t decoded_size_10 = calc_decoded_size(encoded_10);
    REQUIRE(decoded_size_10 == expected_size_10);

    const size_t decoded_size_11 = calc_decoded_size(encoded_11);
    REQUIRE(decoded_size_11 == expected_size_11);

    const size_t decoded_size_12 = calc_decoded_size(encoded_12);
    REQUIRE(decoded_size_12 == expected_size_12);

    std::string decoded;
    decoded.resize(expected_size_1);
    error_code_t code = decode(encoded_1, decoded);
    REQUIRE(code);
    REQUIRE(decoded == expected_result_1);

    decoded.clear();
    decoded.resize(expected_size_2);
    code = decode(encoded_2, decoded);
    REQUIRE(code);
    REQUIRE(decoded == expected_result_2);

    decoded.clear();
    decoded.resize(expected_size_3);
    code = decode(encoded_3, decoded);
    REQUIRE(code);
    REQUIRE(decoded == expected_result_3);

    decoded.clear();
    decoded.resize(expected_size_10);
    code = decode(encoded_10, decoded);
    REQUIRE(code);
    REQUIRE(decoded == expected_result_10);

    decoded.clear();
    decoded.resize(expected_size_11);
    code = decode(encoded_11, decoded);
    REQUIRE(code);
    REQUIRE(decoded == expected_result_11);

    decoded.clear();
    decoded.resize(expected_size_12);
    code = decode(encoded_12, decoded);
    REQUIRE(code);
    REQUIRE(decoded == expected_result_12);
}


TEST_CASE("encode_to_different_storages")
{
    using namespace base64;

    constexpr std::string_view data = "012345678910";
    constexpr size_t expected_size = 16;
    constexpr std::string_view expected_result = "MDEyMzQ1Njc4OTEw";

    const size_t encoded_size = calc_encoded_size(data.size());
    REQUIRE(encoded_size == expected_size);

    std::string encoded_string;
    encoded_string.resize(encoded_size);
    const error_code_t code_1 = encode(data, encoded_string);
    REQUIRE(code_1);
    REQUIRE(encoded_string == expected_result);

    char encoded_char_array[expected_size];
    const error_code_t code_2 = encode(data, encoded_char_array);
    REQUIRE(code_2);
    REQUIRE(strncmp(encoded_char_array, expected_result.data(), expected_size) == 0);

    std::array<uint8_t, expected_size> encoded_array;
    const error_code_t code_3 = encode(data, encoded_array);
    REQUIRE(code_3);
    REQUIRE(memcmp(encoded_array.data(), expected_result.data(), expected_size) == 0);

    std::vector<uint8_t> encoded_vector;
    encoded_vector.resize(encoded_size);
    const error_code_t code_4 = encode(data, encoded_vector);
    REQUIRE(code_4);
    REQUIRE(memcmp(encoded_vector.data(), expected_result.data(), expected_size) == 0);
}


TEST_CASE("decode_to_different_storages")
{
    using namespace base64;

    constexpr std::string_view encoded = "MDEyMzQ1Njc4OTEw";
    constexpr size_t expected_size = 12;
    constexpr std::string_view expected_result = "012345678910";

    const size_t decoded_size = calc_decoded_size(encoded);
    REQUIRE(decoded_size == expected_size);

    std::string decoded_string;
    decoded_string.resize(decoded_size);
    const error_code_t code_1 = decode(encoded, decoded_string);
    REQUIRE(code_1);
    REQUIRE(decoded_string == expected_result);

    char decoded_char_array[expected_size];
    const error_code_t code_2 = decode(encoded, decoded_char_array);
    REQUIRE(code_2);
    REQUIRE(strncmp(decoded_char_array, expected_result.data(), expected_size) == 0);

    std::array<uint8_t, expected_size> encoded_array;
    const error_code_t code_3 = decode(encoded, encoded_array);
    REQUIRE(code_3);
    REQUIRE(memcmp(encoded_array.data(), expected_result.data(), expected_size) == 0);

    std::vector<uint8_t> decoded_vector;
    decoded_vector.resize(decoded_size);
    const error_code_t code_4 = decode(encoded, decoded_vector);
    REQUIRE(code_4);
    REQUIRE(memcmp(decoded_vector.data(), expected_result.data(), expected_size) == 0);
}


TEST_CASE("encode_from_different_storages")
{
    using namespace base64;

    constexpr size_t data_size = 12;
    constexpr char data_char_array[data_size] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '1', '0' };
    constexpr std::array<int8_t, data_size> data_array = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '1', '0' };
    const std::string data_string = "012345678910";
    constexpr std::string_view data_string_view = "012345678910";
    const std::vector<uint8_t> data_vector = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '1', '0' };
    constexpr const char * data_literals = "012345678910"; // GCC wants const
    constexpr size_t expected_size = 16;
    constexpr std::string_view expected_result = "MDEyMzQ1Njc4OTEw";

    const size_t encoded_size = calc_encoded_size(data_string.size());
    REQUIRE(encoded_size == expected_size);

    std::string encoded;
    encoded.resize(encoded_size);
    error_code_t code = encode(data_char_array, encoded);
    REQUIRE(code);
    REQUIRE(encoded == expected_result);

    encoded.erase();
    encoded.resize(encoded_size);
    code = encode(data_array, encoded);
    REQUIRE(code);
    REQUIRE(encoded == expected_result);

    encoded.erase();
    encoded.resize(encoded_size);
    code = encode(data_string, encoded);
    REQUIRE(code);
    REQUIRE(encoded == expected_result);

    encoded.erase();
    encoded.resize(encoded_size);
    code = encode(data_string_view, encoded);
    REQUIRE(code);
    REQUIRE(encoded == expected_result);

    encoded.erase();
    encoded.resize(encoded_size);
    code = encode(data_vector, encoded);
    REQUIRE(code);
    REQUIRE(encoded == expected_result);

    encoded.erase();
    encoded.resize(encoded_size);
    code = encode(make_const_adapter(data_literals, strlen(data_literals)), encoded);
    REQUIRE(code);
    REQUIRE(encoded == expected_result);
}


TEST_CASE("decode_from_different_storages")
{
    using namespace base64;

    constexpr size_t encoded_size = 16;
    constexpr char encoded_char_array[] = { 'M', 'D', 'E', 'y', 'M', 'z', 'Q', '1', 'N', 'j', 'c', '4', 'O', 'T', 'E', 'w' };
    constexpr std::array<char, encoded_size> encoded_array = { 'M', 'D', 'E', 'y', 'M', 'z', 'Q', '1', 'N', 'j', 'c', '4', 'O', 'T', 'E', 'w' };
    const std::string encoded_string = "MDEyMzQ1Njc4OTEw";
    constexpr std::string_view encoded_string_view = "MDEyMzQ1Njc4OTEw";
    const std::vector<uint8_t> encoded_vector = { 'M', 'D', 'E', 'y', 'M', 'z', 'Q', '1', 'N', 'j', 'c', '4', 'O', 'T', 'E', 'w' };
    constexpr const char * encoded_literals = "MDEyMzQ1Njc4OTEw";  // GCC wants const
    constexpr size_t expected_size = 12;
    constexpr std::string_view expected_result = "012345678910";

    const size_t decoded_size = calc_decoded_size(encoded_string);
    REQUIRE(decoded_size == expected_size);

    std::string decoded;
    decoded.resize(decoded_size);
    error_code_t code = decode(encoded_char_array, decoded);
    REQUIRE(code);
    REQUIRE(decoded == expected_result);

    decoded.erase();
    decoded.resize(decoded_size);
    code = decode(encoded_array, decoded);
    REQUIRE(code);
    REQUIRE(decoded == expected_result);

    decoded.erase();
    decoded.resize(decoded_size);
    code = decode(encoded_string, decoded);
    REQUIRE(code);
    REQUIRE(decoded == expected_result);

    decoded.erase();
    decoded.resize(decoded_size);
    code = decode(encoded_string_view, decoded);
    REQUIRE(code);
    REQUIRE(decoded == expected_result);

    decoded.erase();
    decoded.resize(decoded_size);
    code = decode(encoded_vector, decoded);
    REQUIRE(code);
    REQUIRE(decoded == expected_result);

    decoded.erase();
    decoded.resize(decoded_size);
    code = decode(make_const_adapter(encoded_literals, strlen(encoded_literals)), decoded);
    REQUIRE(code);
    REQUIRE(decoded == expected_result);
}


TEST_CASE("encode_binary_data")
{
    using namespace base64;

    constexpr size_t data_size = 512;
    const std::vector<uint8_t> binary = make_bin_arrya(data_size);

    constexpr size_t expected_size = 684;
    constexpr std::string_view expected_result = 
        "AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8gISIjJCUmJygpKissLS4vMDEyMzQ1Njc4OTo7PD0+P0BBQk"
        "NERUZHSElKS0xNTk9QUVJTVFVWV1hZWltcXV5fYGFiY2RlZmdoaWprbG1ub3BxcnN0dXZ3eHl6e3x9fn+AgYKDhIWG"
        "h4iJiouMjY6PkJGSk5SVlpeYmZqbnJ2en6ChoqOkpaanqKmqq6ytrq+wsbKztLW2t7i5uru8vb6/wMHCw8TFxsfIyc"
        "rLzM3Oz9DR0tPU1dbX2Nna29zd3t/g4eLj5OXm5+jp6uvs7e7v8PHy8/T19vf4+fr7/P3+/wABAgMEBQYHCAkKCwwN"
        "Dg8QERITFBUWFxgZGhscHR4fICEiIyQlJicoKSorLC0uLzAxMjM0NTY3ODk6Ozw9Pj9AQUJDREVGR0hJSktMTU5PUF"
        "FSU1RVVldYWVpbXF1eX2BhYmNkZWZnaGlqa2xtbm9wcXJzdHV2d3h5ent8fX5/gIGCg4SFhoeIiYqLjI2Oj5CRkpOU"
        "lZaXmJmam5ydnp+goaKjpKWmp6ipqqusra6vsLGys7S1tre4ubq7vL2+v8DBwsPExcbHyMnKy8zNzs/Q0dLT1NXW19"
        "jZ2tvc3d7f4OHi4+Tl5ufo6err7O3u7/Dx8vP09fb3+Pn6+/z9/v8=";

    const size_t encoded_size = calc_encoded_size(binary.size());
    REQUIRE(encoded_size == expected_size);

    std::string encoded;
    encoded.resize(encoded_size);
    const error_code_t code = encode(binary, encoded);
    REQUIRE(code);
    REQUIRE(encoded == expected_result);
}


TEST_CASE("decode_binary_data")
{
    using namespace base64;

    constexpr std::string_view encoded =
        "AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8gISIjJCUmJygpKissLS4vMDEyMzQ1Njc4OTo7PD0+P0BBQk"
        "NERUZHSElKS0xNTk9QUVJTVFVWV1hZWltcXV5fYGFiY2RlZmdoaWprbG1ub3BxcnN0dXZ3eHl6e3x9fn+AgYKDhIWG"
        "h4iJiouMjY6PkJGSk5SVlpeYmZqbnJ2en6ChoqOkpaanqKmqq6ytrq+wsbKztLW2t7i5uru8vb6/wMHCw8TFxsfIyc"
        "rLzM3Oz9DR0tPU1dbX2Nna29zd3t/g4eLj5OXm5+jp6uvs7e7v8PHy8/T19vf4+fr7/P3+/wABAgMEBQYHCAkKCwwN"
        "Dg8QERITFBUWFxgZGhscHR4fICEiIyQlJicoKSorLC0uLzAxMjM0NTY3ODk6Ozw9Pj9AQUJDREVGR0hJSktMTU5PUF"
        "FSU1RVVldYWVpbXF1eX2BhYmNkZWZnaGlqa2xtbm9wcXJzdHV2d3h5ent8fX5/gIGCg4SFhoeIiYqLjI2Oj5CRkpOU"
        "lZaXmJmam5ydnp+goaKjpKWmp6ipqqusra6vsLGys7S1tre4ubq7vL2+v8DBwsPExcbHyMnKy8zNzs/Q0dLT1NXW19"
        "jZ2tvc3d7f4OHi4+Tl5ufo6err7O3u7/Dx8vP09fb3+Pn6+/z9/v8=";

    constexpr size_t expected_size = 512;
    const std::vector<uint8_t> expected_result = make_bin_arrya(expected_size);

    const size_t decoded_size = calc_decoded_size(encoded);
    REQUIRE(decoded_size == expected_size);

    std::vector<uint8_t> decoded;
    decoded.resize(decoded_size);
    const error_code_t code = decode(encoded, decoded);
    REQUIRE(code);
    REQUIRE(decoded == expected_result);
}


TEST_CASE("encode_utf8")
{
    using namespace base64;

    constexpr std::basic_string_view<char8_t> russian =
        u8"«Любая достаточно развитая технология неотличима от магии» — третий закон Артура Кларка.";
    constexpr size_t russian_size = russian.size();

    constexpr std::string_view russian_base64 =
        "wqvQm9GO0LHQsNGPINC00L7RgdGC0LDRgtC+0YfQvdC+INGA0LDQt9Cy0LjRgtCw0Y8g0YLQtdGF0L3QvtC70L7Qs9"
        "C40Y8g0L3QtdC+0YLQu9C40YfQuNC80LAg0L7RgiDQvNCw0LPQuNC4wrsg4oCUINGC0YDQtdGC0LjQuSDQt9Cw0LrQ"
        "vtC9INCQ0YDRgtGD0YDQsCDQmtC70LDRgNC60LAu";
    constexpr size_t russian_base64_size = russian_base64.size();

    constexpr std::basic_string_view<char8_t> japanese = u8"かたつぶりそろそろ登れ富士の山";
    constexpr size_t japanese_size = japanese.size();

    constexpr std::string_view japanese_base64 = 
        "44GL44Gf44Gk44G244KK44Gd44KN44Gd44KN55m744KM5a+M5aOr44Gu5bGx";
    constexpr size_t japanese_base64_size = japanese_base64.size();

    size_t encoded_size = calc_encoded_size(russian_size);
    REQUIRE(encoded_size == russian_base64_size);

    std::string encoded;
    encoded.resize(encoded_size);
    error_code_t code = encode(russian, encoded);
    REQUIRE(code);
    REQUIRE(encoded == russian_base64);

    encoded_size = calc_encoded_size(japanese_size);
    REQUIRE(encoded_size == japanese_base64_size);

    encoded.clear();
    encoded.resize(encoded_size);
    code = encode(japanese, encoded);
    REQUIRE(code);
    REQUIRE(encoded == japanese_base64);
}


TEST_CASE("decode_utf8")
{
    using namespace base64;

    constexpr std::basic_string_view<char8_t> russian =
        u8"«Любая достаточно развитая технология неотличима от магии» — третий закон Артура Кларка.";
    constexpr size_t russian_size = russian.size();

    constexpr std::string_view russian_base64 =
        "wqvQm9GO0LHQsNGPINC00L7RgdGC0LDRgtC+0YfQvdC+INGA0LDQt9Cy0LjRgtCw0Y8g0YLQtdGF0L3QvtC70L7Qs9"
        "C40Y8g0L3QtdC+0YLQu9C40YfQuNC80LAg0L7RgiDQvNCw0LPQuNC4wrsg4oCUINGC0YDQtdGC0LjQuSDQt9Cw0LrQ"
        "vtC9INCQ0YDRgtGD0YDQsCDQmtC70LDRgNC60LAu";

    constexpr std::basic_string_view<char8_t> japanese = u8"かたつぶりそろそろ登れ富士の山";
    constexpr size_t japanese_size = japanese.size();

    constexpr std::string_view japanese_base64 =
        "44GL44Gf44Gk44G244KK44Gd44KN44Gd44KN55m744KM5a+M5aOr44Gu5bGx";

    size_t decoded_size = calc_decoded_size(russian_base64);
    REQUIRE(decoded_size == russian_size);

    std::basic_string<char8_t> decoded;
    decoded.resize(decoded_size);
    error_code_t code = decode(russian_base64, decoded);
    REQUIRE(code);
    REQUIRE(decoded == russian);

    decoded_size = calc_decoded_size(japanese_base64);
    REQUIRE(decoded_size == japanese_size);

    decoded.clear();
    decoded.resize(decoded_size);
    code = decode(japanese_base64, decoded);
    REQUIRE(code);
    REQUIRE(decoded == japanese);
}
