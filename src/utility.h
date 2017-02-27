#ifndef UTILITY_H
#define UTILITY_H

#include <sstream>
#include <string>
#include <functional>

namespace minecpp
{
    namespace utility
    {
        std::string readFile(const std::string& filepath);

        template<typename T>
        T convert(const std::string& str) noexcept
        {
            std::istringstream sstream{str};
            T value;
            sstream >> value;
            return value;
        }

        template<typename T>
        inline void hash_combine(std::size_t& seed, const T& v) noexcept
        {
            seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
    }
}

#endif
