#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <cassert>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

#include "doctest/doctest.h"
#include "base64.h"


std::vector<uint8_t> make_bin_arrya(size_t count)
{
    std::vector<uint8_t> data(count);
    constexpr size_t max = std::numeric_limits<uint8_t>::max() + 1;

    for (size_t i = 0; i < count; ++i)
        data[i] = static_cast<uint8_t>(i % max);

    return data;
};


TEST_CASE("encode_empty_data")
{
    const std::string_view data;
    constexpr size_t expected_size = 0;
    const size_t encoded_size = base64::calc_encoded_size(data.size());
    REQUIRE(encoded_size == expected_size);

    std::string encoded;
    size_t flag = base64::encode(data, encoded);
    REQUIRE(flag == 0);
    REQUIRE(encoded.empty());
}


TEST_CASE("decode_empty_data")
{
    const std::string_view data;
    constexpr size_t expected_size = 0;
    const size_t decoded_size = base64::calc_decoded_size(data);
    REQUIRE(decoded_size == expected_size);

    std::string decoded;
    size_t flag = base64::decode(data, decoded);
    REQUIRE(flag == 0);
    REQUIRE(decoded.empty());
}


TEST_CASE("encode_with_tails_012")
{
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

    const size_t encoded_size_1 = base64::calc_encoded_size(data_1.size());
    REQUIRE(encoded_size_1 == expected_size_1_2_3);

    const size_t encoded_size_2 = base64::calc_encoded_size(data_2.size());
    REQUIRE(encoded_size_2 == expected_size_1_2_3);

    const size_t encoded_size_3 = base64::calc_encoded_size(data_3.size());
    REQUIRE(encoded_size_3 == expected_size_1_2_3);

    const size_t encoded_size_10 = base64::calc_encoded_size(data_10.size());
    REQUIRE(encoded_size_10 == expected_size_10_11_12);

    const size_t encoded_size_11 = base64::calc_encoded_size(data_11.size());
    REQUIRE(encoded_size_11 == expected_size_10_11_12);

    const size_t encoded_size_12 = base64::calc_encoded_size(data_10.size());
    REQUIRE(encoded_size_12 == expected_size_10_11_12);

    std::string encoded;
    encoded.resize(expected_size_1_2_3);
    size_t flag = base64::encode(data_1, encoded);
    REQUIRE(flag == 0);
    REQUIRE(encoded == expected_result_1);

    encoded.clear();
    encoded.resize(expected_size_1_2_3);
    flag = base64::encode(data_2, encoded);
    REQUIRE(flag == 0);
    REQUIRE(encoded == expected_result_2);

    encoded.clear();
    encoded.resize(expected_size_1_2_3);
    flag = base64::encode(data_3, encoded);
    REQUIRE(flag == 0);
    REQUIRE(encoded == expected_result_3);

    encoded.clear();
    encoded.resize(expected_size_10_11_12);
    flag = base64::encode(data_10, encoded);
    REQUIRE(flag == 0);
    REQUIRE(encoded == expected_result_10);

    encoded.clear();
    encoded.resize(expected_size_10_11_12);
    flag = base64::encode(data_11, encoded);
    REQUIRE(flag == 0);
    REQUIRE(encoded == expected_result_11);

    encoded.clear();
    encoded.resize(expected_size_10_11_12);
    flag = base64::encode(data_12, encoded);
    REQUIRE(flag == 0);
    REQUIRE(encoded == expected_result_12);
}


TEST_CASE("decode_with_tails_012")
{
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

    const size_t decoded_size_1 = base64::calc_decoded_size(encoded_1);
    REQUIRE(decoded_size_1 == expected_size_1);

    const size_t decoded_size_2 = base64::calc_decoded_size(encoded_2);
    REQUIRE(decoded_size_2 == expected_size_2);

    const size_t decoded_size_3 = base64::calc_decoded_size(encoded_3);
    REQUIRE(decoded_size_3 == expected_size_3);

    const size_t decoded_size_10 = base64::calc_decoded_size(encoded_10);
    REQUIRE(decoded_size_10 == expected_size_10);

    const size_t decoded_size_11 = base64::calc_decoded_size(encoded_11);
    REQUIRE(decoded_size_11 == expected_size_11);

    const size_t decoded_size_12 = base64::calc_decoded_size(encoded_12);
    REQUIRE(decoded_size_12 == expected_size_12);

    std::string decoded;
    decoded.resize(expected_size_1);
    size_t flag = base64::decode(encoded_1, decoded);
    REQUIRE(flag == 0);
    REQUIRE(decoded == expected_result_1);

    decoded.clear();
    decoded.resize(expected_size_2);
    flag = base64::decode(encoded_2, decoded);
    REQUIRE(flag == 0);
    REQUIRE(decoded == expected_result_2);

    decoded.clear();
    decoded.resize(expected_size_3);
    flag = base64::decode(encoded_3, decoded);
    REQUIRE(flag == 0);
    REQUIRE(decoded == expected_result_3);

    decoded.clear();
    decoded.resize(expected_size_10);
    flag = base64::decode(encoded_10, decoded);
    REQUIRE(flag == 0);
    REQUIRE(decoded == expected_result_10);

    decoded.clear();
    decoded.resize(expected_size_11);
    flag = base64::decode(encoded_11, decoded);
    REQUIRE(flag == 0);
    REQUIRE(decoded == expected_result_11);

    decoded.clear();
    decoded.resize(expected_size_12);
    flag = base64::decode(encoded_12, decoded);
    REQUIRE(flag == 0);
    REQUIRE(decoded == expected_result_12);
}


TEST_CASE("encode_to_different_storages")
{
    constexpr std::string_view data = "012345678910";
    constexpr size_t expected_size = 16;
    constexpr std::string_view expected_result = "MDEyMzQ1Njc4OTEw";

    const size_t encoded_size = base64::calc_encoded_size(data.size());
    REQUIRE(encoded_size == expected_size);

    std::string encoded_string;
    encoded_string.resize(encoded_size);
    const size_t flag_1 = base64::encode(data, encoded_string);
    REQUIRE(flag_1 == 0);
    REQUIRE(encoded_string == expected_result);

    char encoded_char_array[expected_size];
    const size_t flag_2 = base64::encode(data, encoded_char_array);
    REQUIRE(flag_2 == 0);
    REQUIRE(strncmp(encoded_char_array, expected_result.data(), expected_size) == 0);

    std::vector<uint8_t> encoded_vector;
    encoded_vector.resize(encoded_size);
    const size_t flag_3 = base64::encode(data, encoded_vector);
    REQUIRE(flag_3 == 0);
    REQUIRE(memcmp(encoded_vector.data(), expected_result.data(), expected_size) == 0);
}


TEST_CASE("decode_to_different_storages")
{
    constexpr std::string_view encoded = "MDEyMzQ1Njc4OTEw";
    constexpr size_t expected_size = 12;
    constexpr std::string_view expected_result = "012345678910";

    const size_t decoded_size = base64::calc_decoded_size(encoded);
    REQUIRE(decoded_size == expected_size);

    std::string decoded_string;
    decoded_string.resize(decoded_size);
    const size_t flag_1 = base64::decode(encoded, decoded_string);
    REQUIRE(flag_1 == 0);
    REQUIRE(decoded_string == expected_result);

    char decoded_char_array[expected_size];
    const size_t flag_2 = base64::decode(encoded, decoded_char_array);
    REQUIRE(flag_2 == 0);
    REQUIRE(strncmp(decoded_char_array, expected_result.data(), expected_size) == 0);

    std::vector<uint8_t> decoded_vector;
    decoded_vector.resize(decoded_size);
    const size_t flag_3 = base64::decode(encoded, decoded_vector);
    REQUIRE(flag_3 == 0);
    REQUIRE(memcmp(decoded_vector.data(), expected_result.data(), expected_size) == 0);
}


TEST_CASE("encode_from_different_storages")
{
    const char data_char_array[12] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '1', '0' };
    const std::string data_string = "012345678910";
    constexpr std::string_view data_string_view = "012345678910";
    const std::vector<uint8_t> data_vector = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '1', '0' };
    constexpr const char * data_literals = "012345678910"; // GCC wants const
    constexpr size_t expected_size = 16;
    constexpr std::string_view expected_result = "MDEyMzQ1Njc4OTEw";

    const size_t encoded_size = base64::calc_encoded_size(data_string.size());
    REQUIRE(encoded_size == expected_size);

    std::string encoded;
    encoded.resize(encoded_size);
    size_t flag = base64::encode(data_char_array, encoded);
    REQUIRE(flag == 0);
    REQUIRE(encoded == expected_result);

    encoded.erase();
    encoded.resize(encoded_size);
    flag = base64::encode(data_string, encoded);
    REQUIRE(flag == 0);
    REQUIRE(encoded == expected_result);

    encoded.erase();
    encoded.resize(encoded_size);
    flag = base64::encode(data_string_view, encoded);
    REQUIRE(flag == 0);
    REQUIRE(encoded == expected_result);

    encoded.erase();
    encoded.resize(encoded_size);
    flag = base64::encode(data_vector, encoded);
    REQUIRE(flag == 0);
    REQUIRE(encoded == expected_result);

    encoded.erase();
    encoded.resize(encoded_size);
    flag = base64::encode(base64::make_const_buffer(data_literals, strlen(data_literals)), encoded);
    REQUIRE(flag == 0);
    REQUIRE(encoded == expected_result);
}


TEST_CASE("decode_from_different_storages")
{
    constexpr char encoded_char_array[] = { 'M', 'D', 'E', 'y', 'M', 'z', 'Q', '1', 'N', 'j', 'c', '4', 'O', 'T', 'E', 'w' };
    const std::string encoded_string = "MDEyMzQ1Njc4OTEw";
    constexpr std::string_view encoded_string_view = "MDEyMzQ1Njc4OTEw";
    const std::vector<uint8_t> encoded_vector = { 'M', 'D', 'E', 'y', 'M', 'z', 'Q', '1', 'N', 'j', 'c', '4', 'O', 'T', 'E', 'w' };
    constexpr const char * encoded_literals = "MDEyMzQ1Njc4OTEw";  // GCC wants const
    constexpr size_t expected_size = 12;
    constexpr std::string_view expected_result = "012345678910";

    const size_t decoded_size = base64::calc_decoded_size(encoded_string);
    REQUIRE(decoded_size == expected_size);

    std::string decoded;
    decoded.resize(decoded_size);
    size_t flag = base64::decode(encoded_char_array, decoded);
    REQUIRE(flag == 0);
    REQUIRE(decoded == expected_result);

    decoded.erase();
    decoded.resize(decoded_size);
    flag = base64::decode(encoded_string, decoded);
    REQUIRE(flag == 0);
    REQUIRE(decoded == expected_result);

    decoded.erase();
    decoded.resize(decoded_size);
    flag = base64::decode(encoded_string_view, decoded);
    REQUIRE(flag == 0);
    REQUIRE(decoded == expected_result);

    decoded.erase();
    decoded.resize(decoded_size);
    flag = base64::decode(encoded_vector, decoded);
    REQUIRE(flag == 0);
    REQUIRE(decoded == expected_result);

    decoded.erase();
    decoded.resize(decoded_size);
    flag = base64::decode(base64::make_const_buffer(encoded_literals, strlen(encoded_literals)), decoded);
    REQUIRE(flag == 0);
    REQUIRE(decoded == expected_result);
}


TEST_CASE("encode_binary_data")
{
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

    const size_t encoded_size = base64::calc_encoded_size(binary.size());
    REQUIRE(encoded_size == expected_size);

    std::string encoded;
    encoded.resize(encoded_size);
    const size_t flag = base64::encode(binary, encoded);
    REQUIRE(flag == 0);
    REQUIRE(encoded == expected_result);
}


TEST_CASE("decode_binary_data")
{
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

    const size_t decoded_size = base64::calc_decoded_size(encoded);
    REQUIRE(decoded_size == expected_size);

    std::vector<uint8_t> decoded;
    decoded.resize(decoded_size);
    const size_t flag = base64::decode(encoded, decoded);
    REQUIRE(flag == 0);
    REQUIRE(decoded == expected_result);
}
