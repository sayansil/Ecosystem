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

//enum class Gender : uint8_t
//{
//    MALE, FEMALE
//};

#define FEMALE 1
#define MALE 0

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

    std::ostream& operator <<(std::ostream& out)const
    {
        out << getString();
        return out;
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

std::ostream& operator <<(std::ostream& out, const STAT& stat)
{
    out << stat.getString();
    return out;
}

#endif // ECOSYSTEMTYPES_HPP
