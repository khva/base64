#pragma once

#include <limits>
#include <string_view>
#include <vector>


namespace base64
{

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // custom_encoding_traits struct (definition)
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <const char * alphabet_type, char pad_type>
    struct custom_encoding_traits
    {
        static consteval std::string_view alphabet()    {   return std::string_view{ alphabet_type };           }
        static constexpr uint8_t char_at(size_t index)  {   return static_cast<uint8_t>(alphabet_type[index]);  }
        static consteval bool has_pad()                 {   return pad_type != 0;                               }
        static consteval uint8_t pad()                  {   return static_cast<uint8_t>(pad_type);              }
        static consteval size_t alphabet_size()         {   return alphabet().size();                           }
        static consteval uint32_t invalid_index()       {   return static_cast<uint32_t>(alphabet_size());      }

        static uint32_t index_of(uint8_t symbol)
        {
            auto make_indexes = []() -> std::vector<uint32_t>
            {
                std::vector<uint32_t> indexes(256, invalid_index());

                for (uint32_t i = 0; i < alphabet_size(); ++i)
                    indexes[char_at(i)] = i;

                return indexes;
            };

            static const std::vector<uint32_t> indexes = make_indexes();
            return indexes[symbol];
        }

        static_assert(alphabet_size() == 64, "The BASE64 alphabet must be 64 characters long.");
    };


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // def_encoding_traits struct (definition)
    ////////////////////////////////////////////////////////////////////////////////////////////////

//    constexpr const char def_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//    using def_encoding_traits = custom_encoding_traits<def_alphabet, '='>;
    struct def_encoding_traits
    {
        static consteval std::string_view alphabet()
        {
            return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        }

        static constexpr uint8_t char_at(const size_t index)
        {
            return static_cast<uint8_t>(alphabet()[index]);
        }

        static consteval bool has_pad()             {   return true;    }
        static consteval uint8_t pad()              {   return '=';     }
        static consteval size_t alphabet_size()     {   return 64;      }
        static consteval uint32_t invalid_index()   {   return 64;      }

        static uint32_t index_of(const uint8_t symbol)
        {
            static constinit const uint32_t ii = invalid_index();
            static constinit const uint32_t indexes[] = {
                 ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, //   0-15
                 ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, //  16-31
                 ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, 62, ii, ii, ii, 63, //  32-47
                 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, ii, ii, ii, ii, ii, ii, //  48-63
                 ii,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, //  64-79
                 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, ii, ii, ii, ii, ii, //  80-95
                 ii, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, //  96-111
                 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, ii, ii, ii, ii, ii, // 112-127
                 ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, // 128-143
                 ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, // 144-159
                 ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, // 160-175
                 ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, // 176-191
                 ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, // 192-207
                 ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, // 208-223
                 ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, // 224-239
                 ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii, ii  // 240-255
            };

            return indexes[symbol];
        }
    };

}   // namespace base64
