#ifndef HELPER_HPP
#define HELPER_HPP

#include <string>
#include <utility>
#include <random>
#include <vector>
#include <iostream>
#include <ecosystem_types.hpp>
#include <fstream>
#include <experimental/filesystem>
#include <cassert>

namespace helper
{
    /**********************
     *  Helper attributes *
     **********************/

    extern unsigned int map_height;
    extern unsigned int map_width;
    extern std::mt19937_64 rng;

    /*******************
     *  Helper methods *
     *******************/

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
    extern double get_value_from_chromosome(const std::string &, const unsigned int &, const unsigned int &, const double &);
    extern std::experimental::filesystem::path get_ecosystem_root();
    extern bool is_nonzero_nonnan(const double &);
};

#endif // HELPER_HPP
