#pragma once

#include <cassert>
#include <stdint.h>
#include <utility>


namespace base64
{

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // mutable_buffer class (declaration)
    ////////////////////////////////////////////////////////////////////////////////////////////////

    class mutable_buffer
    {
    public:
        mutable_buffer() = default;
        ~mutable_buffer() = default;

        mutable_buffer(uint8_t * data, size_t byte_count);

        mutable_buffer(const mutable_buffer & other) = default;
        mutable_buffer & operator=(const mutable_buffer & other) = default;

        void swap(mutable_buffer & other);

        uint8_t * data() const;
        size_t size() const;

    private:
        uint8_t   * m_data = nullptr;
        size_t      m_size = 0;
    };


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // const_buffer class (declaration)
    ////////////////////////////////////////////////////////////////////////////////////////////////

    class const_buffer
    {
    public:
        const_buffer() = default;
        ~const_buffer() = default;

        const_buffer(const uint8_t * data, size_t byte_count);

        const_buffer(const const_buffer & other) = default;
        const_buffer & operator=(const const_buffer & other) = default;

        const_buffer(const mutable_buffer & other);
        const_buffer & operator=(const mutable_buffer & other);

        void swap(const_buffer & other);

        const uint8_t * data() const;
        size_t size() const;

    private:
        const uint8_t   * m_data = nullptr;
        size_t            m_size = 0;
    };



    ////////////////////////////////////////////////////////////////////////////////////////////////
    // mutable_buffer class (definition)
    ////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline mutable_buffer::mutable_buffer(uint8_t * data, size_t byte_count)
        : m_data(data)
        , m_size(byte_count)
    {
        assert(data != nullptr || (data == nullptr && byte_count == 0));
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline void mutable_buffer::swap(mutable_buffer & other)
    {
        std::swap(m_data, other.m_data);
        std::swap(m_size, other.m_size);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline uint8_t * mutable_buffer::data() const
    {
        return m_data;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline size_t mutable_buffer::size() const
    {
        return m_size;
    }



    ////////////////////////////////////////////////////////////////////////////////////////////////
    // const_buffer class definition
    ////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline const_buffer::const_buffer(const uint8_t * data, size_t byte_count)
        : m_data(data)
        , m_size(byte_count)
    {
        assert(data != nullptr || (data == nullptr && byte_count == 0));
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline const_buffer::const_buffer(const mutable_buffer & other)
        : m_data(other.data())
        , m_size(other.size())
    {
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline const_buffer & const_buffer::operator=(const mutable_buffer & other)
    {
        const_buffer(other).swap(*this);
        return *this;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline void const_buffer::swap(const_buffer & other)
    {
        std::swap(m_data, other.m_data);
        std::swap(m_size, other.m_size);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline const uint8_t * const_buffer::data() const
    {
        return m_data;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline size_t const_buffer::size() const
    {
        return m_size;
    }


} // namespace base64
