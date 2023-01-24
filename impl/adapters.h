#pragma once

#include <cassert>
#include <stdint.h>
#include <utility>


namespace base64
{

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // mutable_adapter_t class declaration
    ////////////////////////////////////////////////////////////////////////////////////////////////

    class mutable_adapter_t
    {
    public:
        mutable_adapter_t() noexcept = default;
        ~mutable_adapter_t() noexcept = default;

        mutable_adapter_t(uint8_t * data, size_t byte_count) noexcept;

        mutable_adapter_t(const mutable_adapter_t & other) noexcept = default;
        mutable_adapter_t & operator=(const mutable_adapter_t & other) noexcept = default;

        void swap(mutable_adapter_t & other) noexcept;

        uint8_t * data() const noexcept;
        size_t size() const noexcept;

    private:
        uint8_t   * m_data = nullptr;
        size_t      m_size = 0;
    };


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // const_adapter_t class declaration
    ////////////////////////////////////////////////////////////////////////////////////////////////

    class const_adapter_t
    {
    public:
        const_adapter_t() noexcept = default;
        ~const_adapter_t() noexcept = default;

        const_adapter_t(const uint8_t * data, size_t byte_count) noexcept ;

        const_adapter_t(const const_adapter_t & other) noexcept = default;
        const_adapter_t & operator=(const const_adapter_t & other) noexcept = default;

        void swap(const_adapter_t & other) noexcept;

        const uint8_t * data() const noexcept;
        size_t size() const noexcept;

    private:
        const uint8_t   * m_data = nullptr;
        size_t            m_size = 0;
    };



    ////////////////////////////////////////////////////////////////////////////////////////////////
    // mutable_adapter_t class definition
    ////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline mutable_adapter_t::mutable_adapter_t(uint8_t * data, size_t byte_count) noexcept
        : m_data(data)
        , m_size(byte_count)
    {
        assert(data != nullptr || (data == nullptr && byte_count == 0));
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline void mutable_adapter_t::swap(mutable_adapter_t & other) noexcept
    {
        std::swap(m_data, other.m_data);
        std::swap(m_size, other.m_size);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline uint8_t * mutable_adapter_t::data() const noexcept
    {
        return m_data;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline size_t mutable_adapter_t::size() const noexcept
    {
        return m_size;
    }



    ////////////////////////////////////////////////////////////////////////////////////////////////
    // const_adapter_t class definition
    ////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline const_adapter_t::const_adapter_t(const uint8_t * data, size_t byte_count) noexcept
        : m_data(data)
        , m_size(byte_count)
    {
        assert(data != nullptr || (data == nullptr && byte_count == 0));
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline void const_adapter_t::swap(const_adapter_t & other) noexcept
    {
        std::swap(m_data, other.m_data);
        std::swap(m_size, other.m_size);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline const uint8_t * const_adapter_t::data() const noexcept
    {
        return m_data;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    inline size_t const_adapter_t::size() const noexcept
    {
        return m_size;
    }


}   // namespace base64
