#pragma once

#include "buffers.h"

#include <string>
#include <vector>


namespace base64
{

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // buffer makers (declaration)
    ////////////////////////////////////////////////////////////////////////////////////////////////

    mutable_buffer make_buffer(void * data, size_t byte_count);
    const_buffer make_buffer(const void * data, size_t byte_count);


    template <typename pod_type, size_t array_size>
    mutable_buffer make_buffer(pod_type (& data)[array_size]);

    template <typename pod_type, size_t array_size>
    const_buffer make_buffer(const pod_type (& data)[array_size]);


    template <typename pod_type, size_t array_size>
    mutable_buffer make_buffer(pod_type (& data)[array_size], size_t byte_count);

    template <typename pod_type, size_t array_size>
    const_buffer make_buffer(const pod_type (& data)[array_size], size_t byte_count);


    template <typename pod_type, size_t array_size>
    mutable_buffer make_buffer(pod_type (& data)[array_size]);

    template <typename pod_type, size_t array_size>
    const_buffer make_buffer(const pod_type (& data)[array_size]);


    template <typename pod_type, typename allocator_type>
    mutable_buffer make_buffer(std::vector<pod_type, allocator_type> & data);

    template <typename pod_type, typename allocator_type>
    const_buffer make_buffer(const std::vector<pod_type, allocator_type> & data);


    template <typename pod_type, typename allocator_type>
    mutable_buffer make_buffer(std::vector<pod_type, allocator_type> & data, size_t byte_count);

    template <typename pod_type, typename allocator_type>
    const_buffer make_buffer(const std::vector<pod_type, allocator_type> & data, size_t byte_count);


    template <typename char_type, typename traits_type, typename allocator_type>
    mutable_buffer make_buffer(std::basic_string<char_type, traits_type, allocator_type> & data);

    template <typename char_type, typename traits_type, typename allocator_type>
    const_buffer make_buffer(const std::basic_string<char_type, traits_type, allocator_type> & data);


    template <typename char_type, typename traits_type, typename allocator_type>
    mutable_buffer make_buffer(
        std::basic_string<char_type, traits_type, allocator_type>           & data,
        size_t                                                                byte_count);

    template <typename char_type, typename traits_type, typename allocator_type>
    const_buffer make_buffer(
        const std::basic_string<char_type, traits_type, allocator_type>     & data,
        size_t                                                                byte_count);



    ////////////////////////////////////////////////////////////////////////////////////////////////
    // buffer makers (definition)
    ////////////////////////////////////////////////////////////////////////////////////////////////

    inline mutable_buffer make_buffer(void * data, size_t byte_count)
    {
        return mutable_buffer(data, byte_count);
    }


    inline const_buffer make_buffer(const void * data, size_t byte_count)
    {
        return const_buffer(data, byte_count);
    }


    template <typename pod_type, size_t array_size>
    inline mutable_buffer make_buffer(pod_type (& data)[array_size])
    {
        return mutable_buffer(data, array_size * sizeof(pod_type));
    }


    template <typename pod_type, size_t array_size>
    inline const_buffer make_buffer(const pod_type (& data)[array_size])
    {
        return const_buffer(data, array_size * sizeof(pod_type));
    }


    template <typename pod_type, size_t array_size>
    inline mutable_buffer make_buffer(pod_type (& data)[array_size], size_t byte_count)
    {
        constexpr size_t array_bytes = array_size * sizeof(pod_type);
        return mutable_buffer(data, byte_count < array_bytes ? byte_count : array_bytes);
    }


    template <typename pod_type, size_t array_size>
    inline const_buffer make_buffer(const pod_type (& data)[array_size], size_t byte_count)
    {
        constexpr size_t array_bytes = array_size * sizeof(pod_type);
        return const_buffer(data, byte_count < array_bytes ? byte_count : array_bytes);
    }


    template <typename pod_type, typename allocator_type>
    inline mutable_buffer make_buffer(std::vector<pod_type, allocator_type> & data)
    {
        return mutable_buffer(data.data(), data.size() * sizeof(pod_type));
    }

    
    template <typename pod_type, typename allocator_type>
    inline const_buffer make_buffer(const std::vector<pod_type, allocator_type> & data)
    {
        return const_buffer(data.data(), data.size() * sizeof(pod_type));
    }

    
    template <typename pod_type, typename allocator_type>
    inline mutable_buffer make_buffer(std::vector<pod_type, allocator_type> & data, size_t byte_count)
    {
        const size_t array_bytes = data.size() * sizeof(pod_type);
        return mutable_buffer(data.data(), byte_count < array_bytes ? byte_count : array_bytes);
    }


    template <typename pod_type, typename allocator_type>
    inline const_buffer make_buffer(const std::vector<pod_type, allocator_type> & data, size_t byte_count)
    {
        const size_t array_bytes = data.size() * sizeof(pod_type);
        return const_buffer(data.data(), byte_count < array_bytes ? byte_count : array_bytes);
    }


    template <typename char_type, typename traits_type, typename allocator_type>
    inline mutable_buffer make_buffer(std::basic_string<char_type, traits_type, allocator_type> & data)
    {
        const size_t array_bytes = data.size() * sizeof(char_type);
        return mutable_buffer(array_bytes > 0 ? &data[0] : nullptr, array_bytes);
    }


    template <typename char_type, typename traits_type, typename allocator_type>
    inline const_buffer make_buffer(const std::basic_string<char_type, traits_type, allocator_type> & data)
    {
        const size_t array_bytes = data.size() * sizeof(char_type);
        return const_buffer(array_bytes > 0 ? &data[0] : nullptr, array_bytes);
    }


    template <typename char_type, typename traits_type, typename allocator_type>
    inline mutable_buffer make_buffer(
        std::basic_string<char_type, traits_type, allocator_type>           & data,
        size_t                                                                byte_count)
    {
        const size_t array_bytes = data.size() * sizeof(char_type);
        return mutable_buffer(
            array_bytes > 0 ? &data[0] : nullptr, 
            byte_count < array_bytes ? byte_count : array_bytes);
    }


    template <typename char_type, typename traits_type, typename allocator_type>
    inline const_buffer make_buffer(
        std::basic_string<char_type, traits_type, allocator_type>           & data,
        size_t                                                                byte_count)
    {
        const size_t array_bytes = data.size() * sizeof(char_type);
        return const_buffer(
            array_bytes > 0 ? &data[0] : nullptr,
            byte_count < array_bytes ? byte_count : array_bytes);
    }


}   // namespace base64
