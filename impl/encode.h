#pragma once


#include "buffers.h"
#include "encoding_traits.h"

#include <cassert>


namespace base64
{

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // encode functions (declaration)
    ////////////////////////////////////////////////////////////////////////////////////////////////

    size_t calc_encoded_size(size_t raw_size);

    template <typename encoding_traits = def_encoding_traits>
    void encode(
        const const_buffer      & raw_data,
        const mutable_buffer    & base64_data);

    template <typename raw_array, typename base64_array, typename encoding_traits = def_encoding_traits>
    base64_array encode(
        const raw_array         & raw_data);


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // encode functions (definition)
    ////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline size_t calc_encoded_size(size_t raw_size)
    {
        return 4 * ((raw_size + 2) / 3);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename encoding_traits>
    void encode(
        const const_buffer      & raw_data,
        const mutable_buffer    & base64_data)
    {
        const size_t raw_size = raw_data.size();
        const size_t encoded_size = calc_encoded_size(raw_size);

        assert(base64_data.size() >= encoded_size);

        const char * encoding_set = encoding_traits::encoding_set();
        const uint8_t * raw_ptr = static_cast<const uint8_t *>(raw_data.data());
        uint8_t * base64_ptr = static_cast<uint8_t *>(base64_data.data());

        for (size_t i = 0, j = 0; i < raw_size;)
        {
            uint32_t octet_a = i < raw_size ? static_cast<uint8_t>(raw_ptr[i++]) : 0;
            uint32_t octet_b = i < raw_size ? static_cast<uint8_t>(raw_ptr[i++]) : 0;
            uint32_t octet_c = i < raw_size ? static_cast<uint8_t>(raw_ptr[i++]) : 0;

            uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

            base64_ptr[j++] = encoding_set[(triple >> 3 * 6) & 0x3F];
            base64_ptr[j++] = encoding_set[(triple >> 2 * 6) & 0x3F];
            base64_ptr[j++] = encoding_set[(triple >> 1 * 6) & 0x3F];
            base64_ptr[j++] = encoding_set[(triple >> 0 * 6) & 0x3F];
        }

        const size_t tail_size = raw_size % 3;

        if (tail_size == 2)
        {
            base64_ptr[encoded_size - 1] = encoding_traits::placeholder();
        }
        else if (tail_size == 1)
        {
            base64_ptr[encoded_size - 1] = encoding_traits::placeholder();
            base64_ptr[encoded_size - 2] = encoding_traits::placeholder();
        }
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename raw_array, typename base64_array, typename encoding_traits>
    base64_array encode(
        const raw_array         & raw_data)
    {
        base64_array base64_data;
        const size_t encoded_size = calc_encoded_size(raw_data.size());

        base64_data.resize(encoded_size);
        encode(
            const_buffer(&raw_data[0], raw_data.size()),
            mutable_buffer(&base64_data[0], base64_data.size()));

        return base64_data;
    }


}
