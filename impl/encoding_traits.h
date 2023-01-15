#pragma once

#include <limits>
#include <string_view>


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
            static constexpr size_t get(const std::string_view alphabet, const char symbol)
            {
                return alphabet[index] == symbol ? index : index_of<index - 1>::get(alphabet, symbol);
            }
        };


        template <>
        struct index_of<0>
        {
            static constexpr size_t get(const std::string_view alphabet, const char symbol)
            {
                return alphabet[0] == symbol ? 0 : invalid_index;
            }
        };

    }



    ////////////////////////////////////////////////////////////////////////////////////////////////
    // def_encoding_traits struct (definition)
    ////////////////////////////////////////////////////////////////////////////////////////////////

    struct def_encoding_traits
    {
        static constexpr bool has_pad()  {  return true;  }
        static constexpr char pad()      {  return '=';   }

        static constexpr std::string_view alphabet()
        {
            return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        }

        static constexpr size_t alphabet_size() { return alphabet().size(); }

        static constexpr size_t invalid_index = detail::invalid_index;

        static constexpr size_t index_of(const char symbol)
        {
            return detail::index_of<alphabet_size() - 1>::get(alphabet(), symbol);
        }
    };

}   // namespace base64

