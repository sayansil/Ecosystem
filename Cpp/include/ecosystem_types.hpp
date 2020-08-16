#ifndef ECOSYSTEMTYPES_HPP
#define ECOSYSTEMTYPES_HPP

#include <map>
#include <variant>
#include <string>
#include <unordered_map>
#include <refl.hpp>

struct PStat;
using CHROMOSOME_MAP_TYPE = std::map<std::string, std::map<std::string, unsigned int>>;
using ATTRIBUTE_MAP_TYPE = std::unordered_map<std::string, std::unordered_map<std::string, PStat>>;
using ATTRIBUTE_RAW_MAP_TYPE = std::unordered_map<std::string, PStat>;

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

struct PStat
{
    void *address;
    uint8_t index;
    PStat() = default;
    PStat(int& data) : address(&data), index(1) {}
    PStat(double& data) : address(&data), index(2) {}
    PStat(std::string& data) : address(&data), index(3) {}
    PStat(bool& data) : address(&data), index(4) {}
    PStat(char& data) : address(&data), index(5) {}
    PStat(unsigned int& data) : address(&data), index(6) {}

    std::string getString() const
    {
        if(index == 1)
            return std::to_string(*static_cast<int*>(address));
        else if(index == 2)
            return std::to_string(*static_cast<double*>(address));
        else if(index == 3)
            return std::string(*static_cast<std::string*>(address));
        else if(index == 4)
            return std::to_string(*static_cast<bool*>(address));
        else if(index == 5)
            return std::string(1, *static_cast<char*>(address));
        else if(index == 6)
            return std::to_string(*static_cast<unsigned int*>(address));
        else
            throw std::runtime_error("getString() : index not initialized in PStat");
    }

    int getInt() const
    {
        if (index == 1)
            return *static_cast<int *>(address);
        else if (index == 2)
            return (int)(*static_cast<double *>(address));
        else if (index == 6)
            return (int)(*static_cast<unsigned int *>(address));
        else
            throw std::runtime_error("getInt() : Could not convert to specified data type");
    }

    double getDouble() const
    {
        if (index == 1)
            return (double)(*static_cast<int *>(address));
        else if (index == 2)
            return *static_cast<double *>(address);
        else if (index == 6)
            return (double)(*static_cast<unsigned int *>(address));
        else
            throw std::runtime_error("getDouble() : Could not convert to specified data type");
    }

    bool getBool() const
    {
        if (index == 4)
            return *static_cast<bool *>(address);
        else
            throw std::runtime_error("getBool() : Could not convert to specified data type");
    }

    char getChar() const
    {
        if (index == 5)
            return *static_cast<char *>(address);
        else
            throw std::runtime_error("getChar() : Could not convert to specified data type");
    }

    unsigned int getUnsignedInt() const
    {
        if (index == 1)
            return (unsigned int)(*static_cast<int *>(address));
        else if (index == 2)
            return (unsigned int)(*static_cast<double *>(address));
        else if (index == 6)
            return *static_cast<unsigned int *>(address);
        else
            throw std::runtime_error("getUnsignedInt() : Could not convert to specified data type");
    }

    int getIndex() const
    {
        return (int)index;
    }

    void assign(const std::string &val)
    {
        if(index == 1)
            *static_cast<int*>(address) = std::stoi(val);
        else if(index == 2)
            *static_cast<double*>(address) = std::stod(val);
        else if(index == 3)
            *static_cast<std::string*>(address) = val;
        else if(index == 4)
        {
            if(val == "true")
                *static_cast<bool*>(address) = true;
            else if(val == "false")
                *static_cast<bool*>(address) = false;
            else
                throw std::invalid_argument("assign() : bool value neither true nor false");
        }
        else if(index == 5)
        {
            if(val.length() != 1)
                throw std::length_error("assign() : length of string not equal to 1");
            *static_cast<char*>(address) = val[0];
        }
        if(index == 6)
            *static_cast<unsigned int*>(address) = std::stoul(val);
        else
            throw std::runtime_error("assign() : index not initialized");
    }

    template <typename T>
    operator T()const
    {
        return *static_cast<T*>(address);
    }

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
