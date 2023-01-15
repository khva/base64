#pragma once


#include "buffers.h"
#include "encoding_traits.h"
#include "make_buffer.h"

#include <cassert>


namespace base64
{

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // encode functions (declaration)
    ////////////////////////////////////////////////////////////////////////////////////////////////

    size_t calc_encoded_size(size_t raw_size);

    template <typename encoding_traits = def_encoding_traits>
    size_t encode(
        const const_buffer      & raw_data,
        const mutable_buffer    & base64_data);

    template <typename raw_array, typename base64_array, typename encoding_traits = def_encoding_traits>
    size_t encode(
        const raw_array         & raw_data,
        base64_array            & base64_data);


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
    size_t encode(
        const const_buffer      & raw_data,
        const mutable_buffer    & base64_data)
    {
        const size_t raw_size = raw_data.size();
        const size_t encoded_size = calc_encoded_size(raw_size);

        if (base64_data.size() < encoded_size)
        {
            return encoded_size;
        }

        constexpr std::string_view alphabet = encoding_traits::alphabet();
        const uint8_t * raw_ptr = raw_data.data();
        uint8_t * base64_ptr = base64_data.data();

        for (size_t i = 0, j = 0; i < raw_size;)
        {
            uint32_t octet_a = i < raw_size ? raw_ptr[i++] : uint8_t{ 0 };
            uint32_t octet_b = i < raw_size ? raw_ptr[i++] : uint8_t{ 0 };
            uint32_t octet_c = i < raw_size ? raw_ptr[i++] : uint8_t{ 0 };

            uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

            base64_ptr[j++] = static_cast<uint8_t>(alphabet[(triple >> 3 * 6) & 0x3F]);
            base64_ptr[j++] = static_cast<uint8_t>(alphabet[(triple >> 2 * 6) & 0x3F]);
            base64_ptr[j++] = static_cast<uint8_t>(alphabet[(triple >> 1 * 6) & 0x3F]);
            base64_ptr[j++] = static_cast<uint8_t>(alphabet[(triple >> 0 * 6) & 0x3F]);
        }

        const size_t tail_size = raw_size % 3;
        constexpr const uint8_t pad = static_cast<uint8_t>(encoding_traits::pad());

        if (tail_size == 2)
        {
            base64_ptr[encoded_size - 1] = pad;
        }
        else if (tail_size == 1)
        {
            base64_ptr[encoded_size - 1] = pad;
            base64_ptr[encoded_size - 2] = pad;
        }

        return 0;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename raw_array, typename base64_array, typename encoding_traits>
    size_t encode(
        const raw_array         & raw_data,
        base64_array            & base64_data )
    {
        return encode<encoding_traits>(make_const_buffer(raw_data), make_mutable_buffer(base64_data));
    }


}   // namespace base64
