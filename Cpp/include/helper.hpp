#ifndef HELPER_HPP
#define HELPER_HPP

#include <string>
#include <utility>
#include <random>
#include <vector>
#include <iostream>
#include <ecosystem_types.hpp>
#include <fstream>

namespace helper
{
    /**********************
     *  Helper attributes *
     **********************/

    extern unsigned int map_height;
    extern unsigned int map_width;


    /*******************
     *  Helper methods *
     *******************/

    extern void display_stat(const STAT &);
    extern void save_CSV(const std::string &, const std::string &);
    extern void append_CSV(const std::string &, const std::string &);
    extern int weighted_prob(const double &);
    extern std::pair<unsigned int, unsigned int> random_location();
    extern std::string get_random_mixture(const std::string &, const std::string &);
    extern std::string random_binary(const unsigned int &);
    extern std::string random_name(const int &);
    extern std::string to_binary(const unsigned int&);
    extern unsigned int to_decimal(const std::string &);
    extern double weighted_average(const std::vector<double> &values, const std::vector<double> &weights);
    extern double get_value_from_chromosome(const std::string &, const unsigned int &, const unsigned int &, const double &offset);
};

#endif // HELPER_HPP
