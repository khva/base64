#pragma once

#include <cassert>
#include <stdint.h>
#include <utility>


namespace base64
{

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // mutable_buffer_t class (declaration)
    ////////////////////////////////////////////////////////////////////////////////////////////////

    class mutable_buffer_t
    {
    public:
        mutable_buffer_t() = default;
        ~mutable_buffer_t() = default;

        mutable_buffer_t(uint8_t * data, size_t byte_count);

        mutable_buffer_t(const mutable_buffer_t & other) = default;
        mutable_buffer_t & operator=(const mutable_buffer_t & other) = default;

        void swap(mutable_buffer_t & other);

        uint8_t * data() const;
        size_t size() const;

    private:
        uint8_t   * m_data = nullptr;
        size_t      m_size = 0;
    };


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // const_buffer_t class (declaration)
    ////////////////////////////////////////////////////////////////////////////////////////////////

    class const_buffer_t
    {
    public:
        const_buffer_t() = default;
        ~const_buffer_t() = default;

        const_buffer_t(const uint8_t * data, size_t byte_count);

        const_buffer_t(const const_buffer_t & other) = default;
        const_buffer_t & operator=(const const_buffer_t & other) = default;

        const_buffer_t(const mutable_buffer_t & other);
        const_buffer_t & operator=(const mutable_buffer_t & other);

        void swap(const_buffer_t & other);

        const uint8_t * data() const;
        size_t size() const;

    private:
        const uint8_t   * m_data = nullptr;
        size_t            m_size = 0;
    };



    ////////////////////////////////////////////////////////////////////////////////////////////////
    // mutable_buffer_t class (definition)
    ////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline mutable_buffer_t::mutable_buffer_t(uint8_t * data, size_t byte_count)
        : m_data(data)
        , m_size(byte_count)
    {
        assert(data != nullptr || (data == nullptr && byte_count == 0));
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline void mutable_buffer_t::swap(mutable_buffer_t & other)
    {
        std::swap(m_data, other.m_data);
        std::swap(m_size, other.m_size);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline uint8_t * mutable_buffer_t::data() const
    {
        return m_data;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline size_t mutable_buffer_t::size() const
    {
        return m_size;
    }



    ////////////////////////////////////////////////////////////////////////////////////////////////
    // const_buffer_t class definition
    ////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline const_buffer_t::const_buffer_t(const uint8_t * data, size_t byte_count)
        : m_data(data)
        , m_size(byte_count)
    {
        assert(data != nullptr || (data == nullptr && byte_count == 0));
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline const_buffer_t::const_buffer_t(const mutable_buffer_t & other)
        : m_data(other.data())
        , m_size(other.size())
    {
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline const_buffer_t & const_buffer_t::operator=(const mutable_buffer_t & other)
    {
        const_buffer_t(other).swap(*this);
        return *this;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline void const_buffer_t::swap(const_buffer_t & other)
    {
        std::swap(m_data, other.m_data);
        std::swap(m_size, other.m_size);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline const uint8_t * const_buffer_t::data() const
    {
        return m_data;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline size_t const_buffer_t::size() const
    {
        return m_size;
    }


}   // namespace base64
