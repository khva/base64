#pragma once

#include "buffers.h"
#include "encoding_traits.h"

#include <cassert>


namespace base64
{

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // interface functions declaration
    ////////////////////////////////////////////////////////////////////////////////////////////////


    template <typename encoding_traits = def_encoding_traits>
    size_t calc_decoded_size(
        const const_buffer      & base64_data);

    template <typename encoding_traits = def_encoding_traits>
    void decode(
        const const_buffer      & base64_data,
        const mutable_buffer    & raw_data);

    template <typename raw_array, typename base64_array, typename encoding_traits = def_encoding_traits>
    raw_array decode(
        const base64_array      & base64_data);



    ////////////////////////////////////////////////////////////////////////////////////////////////
    // interface functions definition
    ////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename encoding_traits>
    size_t calc_decoded_size(
        const const_buffer      & base64_data)

    {
        const size_t encoded_size = base64_data.size();
        assert(encoded_size % 4 == 0);

        size_t raw_size = 3 * (encoded_size / 4);

        if (encoded_size > 0)
        {
            const uint8_t * base64_ptr = static_cast<const uint8_t *>(base64_data.data());

            if (base64_ptr[encoded_size - 1] == encoding_traits::placeholder())
            {
                --raw_size;

                if (base64_ptr[encoded_size - 2] == encoding_traits::placeholder())
                    --raw_size;
            }
        }

        return raw_size;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename encoding_traits>
    void decode(
        const const_buffer      & base64_data,
        const mutable_buffer    & raw_data)
    {
        const size_t raw_size = calc_decoded_size(base64_data);

        assert(raw_data.size() >= raw_size);

        const size_t encoded_size = base64_data.size();
        const char spacer = encoding_traits::placeholder();

        auto index_of = [](char symbol) -> uint32_t
        {
            const size_t index = encoding_traits::index_of(symbol);
            return index == encoding_traits::invalid_index ? 0 : static_cast<uint32_t>(index);
        };

        const uint8_t * base64_ptr = static_cast<const uint8_t *>(base64_data.data());
        uint8_t * raw_ptr = static_cast<uint8_t *>(raw_data.data());

        for (size_t i = 0, j = 0; i < encoded_size;)
        {
            uint32_t sextet_a = base64_ptr[i] == spacer ? 0 & i++ : index_of(base64_ptr[i++]);
            uint32_t sextet_b = base64_ptr[i] == spacer ? 0 & i++ : index_of(base64_ptr[i++]);
            uint32_t sextet_c = base64_ptr[i] == spacer ? 0 & i++ : index_of(base64_ptr[i++]);
            uint32_t sextet_d = base64_ptr[i] == spacer ? 0 & i++ : index_of(base64_ptr[i++]);

            uint32_t triple =
                (sextet_a << 3 * 6) + (sextet_b << 2 * 6) + (sextet_c << 1 * 6) + (sextet_d << 0 * 6);

            if (j < raw_size) raw_ptr[j++] = (triple >> 2 * 8) & 0xFF;
            if (j < raw_size) raw_ptr[j++] = (triple >> 1 * 8) & 0xFF;
            if (j < raw_size) raw_ptr[j++] = (triple >> 0 * 8) & 0xFF;
        }
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename raw_array, typename base64_array, typename encoding_traits>
    raw_array decode(
        const base64_array      & base64_data)
    {
        raw_array raw_data;
        const_buffer base64_buffer(&base64_data[0], base64_data.size());
        const size_t raw_size = calc_decoded_size(base64_buffer);

        raw_data.resize(raw_size);
        decode(
            base64_buffer,
            mutable_buffer(&raw_data[0], raw_data.size()));

        return raw_data;
    }

} // namespace base64

