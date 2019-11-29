#pragma once

#include <optional>
#include <array>

namespace util
{

template<typename T, std::size_t Size>
struct array_of_optionals : public std::array<std::optional<T>, Size>
{
    using Super = std::array<std::optional<T>, Size>;
    std::optional<T>& operator[](unsigned int i)
    {
        if(i >= Super::size())
        {
            static std::optional<T> dummy;
            return dummy;
        }
        return Super::operator[](i);
    }
    const std::optional<T>& operator[](unsigned int i) const
    {
        if(i >= Super::size())
        {
            static std::optional<T> dummy;
            return dummy;
        }
        return Super::operator[](i);        
    }
};

} // namespace util
