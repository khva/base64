#pragma once

#include <limits>


namespace base64
{

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // helper struct (definition)
    ////////////////////////////////////////////////////////////////////////////////////////////////

    namespace detail
    {
        constexpr size_t invalid_index = std::numeric_limits<size_t>::max();

        template <size_t index>
        struct index_of
        {
            static constexpr size_t get(const char * const encoding_set, const char symbol)
            {
                return encoding_set[index] == symbol ? index : index_of<index - 1>::get(encoding_set, symbol);
            }
        };


        template <>
        struct index_of<0>
        {
            static constexpr size_t get(const char * const encoding_set, const char symbol)
            {
                return encoding_set[0] == symbol ? 0 : invalid_index;
            }
        };

    }



    ////////////////////////////////////////////////////////////////////////////////////////////////
    // def_encoding_traits struct (definition)
    ////////////////////////////////////////////////////////////////////////////////////////////////

    struct def_encoding_traits
    {
        static constexpr char placeholder() { return '='; }

        static constexpr const char * encoding_set()
        {
            return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        }

        static constexpr size_t encoding_size() { return 64; }

        static constexpr size_t invalid_index = detail::invalid_index;

        static constexpr size_t index_of(const char symbol)
        {
            return detail::index_of<encoding_size() - 1>::get(encoding_set(), symbol);
        }
    };

}   // namespace base64

