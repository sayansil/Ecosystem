#ifndef ECOSYSTEMTYPES_HPP
#define ECOSYSTEMTYPES_HPP

#include <map>
#include <variant>

using CHROMOSOME_MAP_TYPE = std::map<std::string, std::map<std::string, unsigned int>>;

using DB_NAME = std::string;
using DB_KIND = std::string;
using DB_CHROMOSOME = std::string;
using DB_GENERATION = unsigned int;
using DB_IMMUNITY = double;
using DB_GENDER = unsigned int;
using DB_AGE = unsigned int;
using DB_HEIGHT = double;
using DB_WEIGHT = double;
using DB_FITNESS = double;

struct STAT
{
    std::variant<unsigned int, double, std::string, bool> data;

    STAT() = default;

    template <typename T>
    STAT(const T& t) : data(t) {}

    template <typename T>
    operator T()const
    {
        return std::get<T>(data);
    }
};

#endif // ECOSYSTEMTYPES_HPP
