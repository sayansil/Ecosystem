#ifndef ECOSYSTEMTYPES_HPP
#define ECOSYSTEMTYPES_HPP

#include <map>
#include <variant>
#include <string>
#include <unordered_map>
#include <../extern/refl-cpp/refl.hpp>

struct PStat;
struct ATTRIBUTE_RAW_MAP;

using CHROMOSOME_MAP_TYPE = std::map<std::string, std::map<std::string, unsigned int>>;
using ATTRIBUTE_MAP_TYPE = std::unordered_map<std::string, std::unordered_map<std::string, PStat>>;

enum Gender
{
    MALE = 0,
    FEMALE = 1
};

enum class PStatType
{
    DEFAULT,
    INT,
    DOUBLE,
    CHAR,
    UINT,
    STRING,
    BOOL
};

enum class StatGroup
{
    FIX,
    MEAN,
    MISC
};

enum class SQLType
{
    TEXT,
    INT,
    FLOAT
};

struct DBType
{
    SQLType dtype;
    std::string data;

    DBType() = default;
    DBType(const SQLType &x, const std::string &y) : dtype(x), data(y) {}
};

struct PStat
{
    void *address;
    PStatType index = PStatType::DEFAULT;
    PStat() = default;
    PStat(int& data) : address(&data), index(PStatType::INT) {}
    PStat(double& data) : address(&data), index(PStatType::DOUBLE) {}
    PStat(std::string& data) : address(&data), index(PStatType::STRING) {}
    PStat(bool& data) : address(&data), index(PStatType::BOOL) {}
    PStat(char& data) : address(&data), index(PStatType::CHAR) {}
    PStat(unsigned int& data) : address(&data), index(PStatType::UINT) {}

    std::string getString() const
    {
        if(index == PStatType::INT)
            return std::to_string(*static_cast<int*>(address));
        else if(index == PStatType::DOUBLE)
            return std::to_string(*static_cast<double*>(address));
        else if(index == PStatType::STRING)
            return std::string(*static_cast<std::string*>(address));
        else if(index == PStatType::BOOL)
            return std::to_string(*static_cast<bool*>(address));
        else if(index == PStatType::CHAR)
            return std::string(1, *static_cast<char*>(address));
        else if(index == PStatType::UINT)
            return std::to_string(*static_cast<unsigned int*>(address));
        else
            throw std::runtime_error("getString() : index not initialized in PStat");
    }

    PStatType getIndex() const
    {
        return index;
    }

    void assign(const std::string &val)
    {
        if(index == PStatType::INT)
            *static_cast<int*>(address) = std::stoi(val);
        else if(index == PStatType::DOUBLE)
            *static_cast<double*>(address) = std::stod(val);
        else if(index == PStatType::STRING)
            *static_cast<std::string*>(address) = val;
        else if(index == PStatType::BOOL)
        {
            if(val == "true")
                *static_cast<bool*>(address) = true;
            else if(val == "false")
                *static_cast<bool*>(address) = false;
            else
                throw std::invalid_argument("assign() : bool value neither true nor false for val=" + val);
        }
        else if(index == PStatType::CHAR)
        {
            if(val.length() != 1)
                throw std::length_error("assign() : length of string not equal to 1 for val=" + val);
            *static_cast<char*>(address) = val[0];
        }
        else if(index == PStatType::UINT)
            *static_cast<unsigned int*>(address) = std::stoul(val);
        else
            throw std::runtime_error("assign() : index not initialized for val=" + val);
    }

    template <typename T>
    operator T()const
    {
        return *static_cast<T*>(address);
    }

};

struct ATTRIBUTE_RAW_MAP
{
    ATTRIBUTE_RAW_MAP() = default;
    ATTRIBUTE_RAW_MAP(const std::unordered_map<std::string, PStat> &x) : map(x) , is_initialized(true) {}

    const PStat& operator[] (const std::string& key) const
    {
        auto it = map.find(key);
        if(it == map.end())
        {
            std::string msg = "key " + key + " not found in map";
            throw std::runtime_error(msg);
        }
        return it->second;
    }

    std::unordered_map<std::string, PStat> map;
    bool is_initialized = false;
};


/*
 * Stat properties :
 * 1 - Fixed for a species
 * 2 - Fixed for the organism throughout their lifetime
 * 3 - Changes every year
 * 4 - Changes every moment
*/

struct fixed_for_species : refl::attr::usage::field, refl::attr::usage::function {};
struct fixed_for_lifetime : refl::attr::usage::field, refl::attr::usage::function {};
struct changes_every_year : refl::attr::usage::field, refl::attr::usage::function {};
struct changes_every_moment : refl::attr::usage::field, refl::attr::usage::function {};


#endif // ECOSYSTEMTYPES_HPP
