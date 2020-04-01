#ifndef ECOSYSTEMTYPES_HPP
#define ECOSYSTEMTYPES_HPP

#include <map>
#include <variant>

using CHROMOSOME_MAP_TYPE = std::map<std::string, std::map<std::string, unsigned int>>;

enum Gender
{
    MALE = 0,
    FEMALE = 1
};

struct STAT
{
    std::variant<unsigned int, double, std::string, bool> data;

    STAT() = default;

    template <typename T>
    STAT(const T &t) : data(t) {}

    template <typename T>
    operator T()const
    {
        return std::get<T>(data);
    }

    std::string getString()const
    {
        if(data.index() == 0)
            return std::to_string(std::get<unsigned int>(data));
        else if(data.index() == 1)
            return std::to_string(std::get<double>(data));
        else if(data.index() == 2)
            return std::get<std::string>(data);
        else if(data.index() == 3)
            return std::to_string(std::get<bool>(data));
        else
            return "";
    }
};

#endif // ECOSYSTEMTYPES_HPP
