#ifndef HELPER_HPP
#define HELPER_HPP

#include <string>
#include <utility>

namespace helper
{
    int map_width = 100;
    int map_height = 100;
    std::string to_binary(const unsigned int&);
    unsigned int to_decimal(const std::string);
    std::string random_binary(const int&);
    std::string random_name(const int&);
    std::pair<unsigned int, unsigned int> random_location();
    std::string get_value_from_chromosome(const std::string&, const int&, const int&);
};

#endif // HELPER_HPP
