#pragma once

#include "buffers.h"

#include <string>
#include <string_view>
#include <vector>


namespace base64
{

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // buffer makers (declaration)
    ////////////////////////////////////////////////////////////////////////////////////////////////

    mutable_buffer make_mutable_buffer(void * data, size_t byte_count);
    const_buffer make_const_buffer(const void * data, size_t byte_count);

    // dummy functions fo calls like: encode(const_buffer, std::string)
    mutable_buffer make_mutable_buffer(const mutable_buffer & buffer);
    const_buffer make_const_buffer(const const_buffer & buffer);


    template <typename pod_type, size_t array_size>
    mutable_buffer make_mutable_buffer(pod_type (& data)[array_size]);

    template <typename pod_type, size_t array_size>
    const_buffer make_const_buffer(const pod_type (& data)[array_size]);


    template <typename pod_type, typename allocator_type>
    mutable_buffer make_mutable_buffer(std::vector<pod_type, allocator_type> & data);

    template <typename pod_type, typename allocator_type>
    const_buffer make_const_buffer(const std::vector<pod_type, allocator_type> & data);


    template <typename char_type, typename traits_type, typename allocator_type>
    mutable_buffer make_mutable_buffer(std::basic_string<char_type, traits_type, allocator_type> & data);

    template <typename char_type, typename traits_type, typename allocator_type>
    const_buffer make_const_buffer(const std::basic_string<char_type, traits_type, allocator_type> & data);


    template <typename char_type, typename traits_type>
    const_buffer make_const_buffer(const std::basic_string_view<char_type, traits_type> & data);



    ////////////////////////////////////////////////////////////////////////////////////////////////
    // buffer makers (definition)
    ////////////////////////////////////////////////////////////////////////////////////////////////

    inline mutable_buffer make_mutable_buffer(void * data, size_t byte_count)
    {
        return mutable_buffer(reinterpret_cast<uint8_t *>(data), byte_count);
    }


    inline const_buffer make_const_buffer(const void * data, size_t byte_count)
    {
        return const_buffer(reinterpret_cast<const uint8_t *>(data), byte_count);
    }


    inline mutable_buffer make_mutable_buffer(const mutable_buffer & buffer)
    {
        return buffer;
    }


    inline const_buffer make_const_buffer(const const_buffer & buffer)
    {
        return buffer;
    }


    template <typename pod_type, size_t array_size>
    inline mutable_buffer make_mutable_buffer(pod_type (& data)[array_size])
    {
        return mutable_buffer(reinterpret_cast<uint8_t *>(data), array_size * sizeof(pod_type));
    }


    template <typename pod_type, size_t array_size>
    inline const_buffer make_const_buffer(const pod_type (& data)[array_size])
    {
        return const_buffer(reinterpret_cast<const uint8_t *>(data), array_size * sizeof(pod_type));
    }


    template <typename pod_type, typename allocator_type>
    inline mutable_buffer make_mutable_buffer(std::vector<pod_type, allocator_type> & data)
    {
        return mutable_buffer(reinterpret_cast<uint8_t *>(data.data()), data.size() * sizeof(pod_type));
    }

    
    template <typename pod_type, typename allocator_type>
    inline const_buffer make_const_buffer(const std::vector<pod_type, allocator_type> & data)
    {
        return const_buffer(reinterpret_cast<const uint8_t *>(data.data()), data.size() * sizeof(pod_type));
    }


    template <typename char_type, typename traits_type, typename allocator_type>
    inline mutable_buffer make_mutable_buffer(std::basic_string<char_type, traits_type, allocator_type> & data)
    {
        const size_t array_bytes = data.size() * sizeof(char_type);
        return mutable_buffer(array_bytes > 0 ? reinterpret_cast<uint8_t *>(data.data()) : nullptr, array_bytes);
    }


    template <typename char_type, typename traits_type, typename allocator_type>
    inline const_buffer make_const_buffer(const std::basic_string<char_type, traits_type, allocator_type> & data)
    {
        const size_t array_bytes = data.size() * sizeof(char_type);
        return const_buffer(array_bytes > 0 ? reinterpret_cast<const uint8_t *>(data.data()) : nullptr, array_bytes);
    }


    template <typename char_type, typename traits_type>
    inline const_buffer make_const_buffer(const std::basic_string_view<char_type, traits_type> & data)
    {
        const size_t array_bytes = data.size() * sizeof(char_type);
        return const_buffer(array_bytes > 0 ? reinterpret_cast<const uint8_t *>(data.data()) : nullptr, array_bytes);
    }

}   // namespace base64
