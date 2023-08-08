#pragma once

/* <editor-fold desc="MIT License">

Copyright(c) 2018 Robert Osfield

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */

#include <string>
#include <typeinfo>
#include <cstdint>

namespace vsg::detail
{
    // compile time hashing code derived from https://github.com/elanthis/constexpr-hash-demo/blob/c59e42f4270c493494d504a0840425cd4d01e346/test.cpp
    // NOTE: hashing algorithm used is FNV-1a (https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function)

    // FNV-1a constants
    static constexpr uint64_t basis = 14695981039346656037ULL;
    static constexpr uint64_t prime = 1099511628211ULL;

    // compile-time hash helper function
    constexpr uint64_t hash_one(char c, const char* remain, uint64_t value)
    {
        return c == 0 ? value : hash_one(remain[0], remain + 1, (value ^ c) * prime);
    }

    // compile-time hash
    constexpr uint64_t hash(const char* str)
    {
        return hash_one(str[0], str + 1, basis);
    }
} // namespace vsg::detail

namespace vsg
{
    // clang-format off

    template<typename T>
    constexpr const char* type_name() noexcept { return typeid(T).name(); }

    template<typename T>
    constexpr const char* type_name(const T&) noexcept { return type_name<T>(); }

    template<> constexpr const char* type_name<std::string>() noexcept { return "string"; }
    template<> constexpr const char* type_name<bool>() noexcept { return "bool"; }
    template<> constexpr const char* type_name<char>() noexcept { return "char"; }
    template<> constexpr const char* type_name<int8_t>() noexcept { return "int8_t"; }
    template<> constexpr const char* type_name<unsigned char>() noexcept { return "uchar"; }
    template<> constexpr const char* type_name<short>() noexcept { return "short"; }
    template<> constexpr const char* type_name<unsigned short>() noexcept { return "ushort"; }
    template<> constexpr const char* type_name<int>() noexcept { return "int"; }
    template<> constexpr const char* type_name<unsigned int>() noexcept { return "uint"; }
    template<> constexpr const char* type_name<float>() noexcept { return "float"; }
    template<> constexpr const char* type_name<double>() noexcept { return "double"; }

    template<typename T>
    constexpr uint64_t type_hash() noexcept { return vsg::detail::hash(type_name<T>()); }

    template<> constexpr uint64_t type_hash<char>() noexcept { return vsg::detail::hash("char"); }

    // helper define for defining the type_name() for a type within the vsg namespace.
    #define VSG_type_name(T) \
        template<> constexpr const char* type_name<T>() noexcept { return #T; } \
        template<> constexpr const char* type_name<const T>() noexcept { return "const "#T; } \
        template<> constexpr uint64_t type_hash<T>() noexcept { return vsg::detail::hash(#T); }


    // helper define for defining the type_name() for a type in a namespace other than vsg, note must be placed in global namespace.
    #define EVSG_type_name(T) \
        template<> constexpr const char* vsg::type_name<T>() noexcept { return #T; } \
        template<> constexpr const char* vsg::type_name<const T>() noexcept { return "const "#T; } \
        template<> constexpr uint64_t vsg::type_hash<T>() noexcept { return vsg::detail::hash(#T); }

    // clang-format on

} // namespace vsg
