#pragma once

#include "buffers.h"
#include "encoding_traits.h"
#include "make_buffer.h"

#include <cassert>


namespace base64
{

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // interface functions declaration
    ////////////////////////////////////////////////////////////////////////////////////////////////


    template <typename encoding_traits = def_encoding_traits>
    size_t calc_decoded_size(
        const const_buffer      & base64_data);


    template <typename base64_array, typename encoding_traits = def_encoding_traits>
    size_t calc_decoded_size(
        const base64_array      & base64_data);


    template <typename encoding_traits = def_encoding_traits>
    size_t decode(
        const const_buffer      & base64_data,
        const mutable_buffer    & raw_data);


    template <typename base64_array, typename raw_array, typename encoding_traits = def_encoding_traits>
    size_t decode(
        const base64_array      & base64_data,
        raw_array               & raw_data);



    ////////////////////////////////////////////////////////////////////////////////////////////////
    // interface functions definition
    ////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename encoding_traits>
    size_t calc_decoded_size(
        const const_buffer      & base64_data)

    {
        const size_t encoded_size = base64_data.size();
        size_t raw_size = 3 * (encoded_size / 4);

        if constexpr (encoding_traits::has_pad())
        {
            if (encoded_size > 0)
            {
                const uint8_t * base64_ptr = base64_data.data();

                if (base64_ptr[encoded_size - 1] == encoding_traits::pad())
                {
                    --raw_size;

                    if (encoded_size > 1 && base64_ptr[encoded_size - 2] == encoding_traits::pad())
                        --raw_size;
                }
            }
        }
        else
        {
            const size_t tail_size = raw_size % 3;
            raw_size += tail_size == 0 ? 0 : tail_size - 1;
        }

        return raw_size;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename base64_array, typename encoding_traits>
    size_t calc_decoded_size(
        const base64_array      & base64_data)
    {
        return calc_decoded_size<encoding_traits>(make_const_buffer(base64_data));
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename encoding_traits>
    size_t decode(
        const const_buffer      & base64_data,
        const mutable_buffer    & raw_data)
    {
        const size_t raw_size = calc_decoded_size<encoding_traits>(base64_data);

        // TODO: check base64_data size
        if (raw_data.size() < raw_size)
        {
            return raw_size;
        }

        auto index_of = [](uint8_t symbol) -> uint32_t
        {
            if constexpr (encoding_traits::has_pad())
            {
                if (symbol == encoding_traits::pad())
                    return 0;
            }
            const uint32_t index = encoding_traits::index_of(symbol);
            return index == encoding_traits::invalid_index() ? 0 : index;
        };

        auto calc_triple = [](uint32_t sextet_a, uint32_t sextet_b, uint32_t sextet_c, uint32_t sextet_d) -> uint32_t
        {
            return (sextet_a << 3 * 6) + (sextet_b << 2 * 6) + (sextet_c << 1 * 6) + (sextet_d << 0 * 6);
        };

        const size_t encoded_size = 4 * (base64_data.size() / 4);
        const size_t tail_size = base64_data.size() - encoded_size;
        const uint8_t * base64_ptr = base64_data.data();
        uint8_t * raw_ptr = raw_data.data();

        // TODO: stop calculation when find bad symbol
        size_t raw_pos = 0;
        for (size_t i = 0; i < encoded_size;)
        {
            const uint32_t sextet_a = index_of(base64_ptr[i++]);
            const uint32_t sextet_b = index_of(base64_ptr[i++]);
            const uint32_t sextet_c = index_of(base64_ptr[i++]);
            const uint32_t sextet_d = index_of(base64_ptr[i++]);

            const uint32_t triple = calc_triple(sextet_a, sextet_b, sextet_c, sextet_d);

            assert(raw_pos < raw_size);
            raw_ptr[raw_pos++] = (triple >> 2 * 8) & 0xFF;

            if (raw_pos < raw_size)
                raw_ptr[raw_pos++] = (triple >> 1 * 8) & 0xFF;

            if (raw_pos < raw_size)
                raw_ptr[raw_pos++] = (triple >> 0 * 8) & 0xFF;
        }

        if constexpr (!encoding_traits::has_pad())
        {
            assert(tail_size == 0 || tail_size == 2 || tail_size == 3);

            if (tail_size > 0)
            {
                const uint32_t sextet_a = index_of(base64_ptr[encoded_size]);
                const uint32_t sextet_b = tail_size > 1 ? index_of(base64_ptr[encoded_size + 1]) : 0;
                const uint32_t sextet_c = tail_size > 2 ? index_of(base64_ptr[encoded_size + 2]) : 0;

                const uint32_t triple = calc_triple(sextet_a, sextet_b, sextet_c, 0);

                assert(raw_pos < raw_size);
                raw_ptr[raw_pos++] = (triple >> 2 * 8) & 0xFF;
                
                if (raw_pos < raw_size)
                    raw_ptr[raw_pos++] = (triple >> 1 * 8) & 0xFF;
            }
        }

        return 0;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename base64_array, typename raw_array, typename encoding_traits>
    size_t decode(
        const base64_array      & base64_data,
        raw_array               & raw_data)
    {
        return decode<encoding_traits>(make_const_buffer(base64_data), make_mutable_buffer(raw_data));
    }


}   // namespace base64
