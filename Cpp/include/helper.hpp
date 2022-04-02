#ifndef HELPER_HPP
#define HELPER_HPP

#include <string>
#include <utility>
#include <random>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cassert>
#include <XoshiroCpp.hpp>

namespace helper
{
    /**********************
     *  Helper attributes *
     **********************/

    extern unsigned int map_height;
    extern unsigned int map_width;
    extern XoshiroCpp::Xoshiro128PlusPlus rng;
    extern std::filesystem::path ecosystem_root;

    /*******************
     *  Helper methods *
     *******************/

    extern int weighted_prob(const double &);
    extern std::pair<uint64_t, uint64_t> random_location();
    extern std::string get_random_mixture(const std::string &, const std::string &);
    extern std::string random_binary(const unsigned int &);
    extern std::string random_name(const int &);
    extern std::string to_binary(const unsigned int &);
    extern unsigned int to_decimal(const std::string &);
    extern double weighted_average(const std::vector<double> &values, const std::vector<double> &weights);
    extern double get_value_from_chromosome(const std::string &, const unsigned int &, const unsigned int &, const double &);
    extern bool is_nonzero_nonnegative_nonnan(const double &);
    extern std::string generate_full_species_name(const std::string &, const std::string &);
    extern std::vector<uint8_t> string_to_bytevector(const std::string&);
    extern std::string bytevector_to_string(const uint8_t* arr, const size_t& len, const size_t& expected_len);
};

#endif // HELPER_HPP
