#include <helper.hpp>
#include <fmt/core.h>

namespace helper
{
    unsigned int map_height = 1000;
    unsigned int map_width = 1000;
    XoshiroCpp::Xoshiro128PlusPlus rng{std::random_device()()};

    std::string to_binary(const unsigned int &x)
    {
        auto num = x;
        std::string str = "";
        while (num)
        {
            str = std::to_string(num % 2) + str;
            num >>= 1;
        }
        return str;
    }

    unsigned int to_decimal(const std::string &str)
    {
        unsigned int ans = 0;
        auto n = str.length();
        for (int i = n - 1; i >= 0; i--)
        {
            if (str[i] == '1')
                ans = ans + (1 << (n - i - 1));
        }
        return ans;
    }

    std::string random_binary(const unsigned int &n)
    {
        std::string str = "";
        std::uniform_int_distribution<int> dis(0, 1);
        for (unsigned int i = 0; i < n; i++)
            str += std::to_string(dis(rng));
        return str;
    }

    std::string random_name(const int &inp)
    {
        std::string str = "";
        std::uniform_int_distribution<int> dis(0, 35);
        for (int i = 0; i < inp; i++)
        {
            int tmp = dis(rng);
            if (tmp >= 0 && tmp <= 9)
                str += std::to_string(tmp);
            else
                str += static_cast<char>(tmp - 10 + 'a');
        }
        return str;
    }

    std::pair<uint64_t, uint64_t> random_location()
    {
        std::uniform_int_distribution<uint64_t> dis_height(0, static_cast<uint64_t>(map_height - 1));
        std::uniform_int_distribution<uint64_t> dis_width(0, static_cast<uint64_t>(map_width - 1));
        return {dis_width(rng), dis_height(rng)};
    }

    double weighted_average(const std::vector<double> &values, const std::vector<double> &weights)
    {
        double num = 0.0, den = 0.0;
        for (size_t i = 0; i < values.size(); i++)
        {
            num += (values[i] * weights[i]);
            den += weights[i];
        }
        return num / den;
    }

    std::string get_random_mixture(const std::string &str1, const std::string &str2)
    {
        std::string str3;
        std::uniform_int_distribution<int> dis(0, 1);
        int i;
        for (i = 0; i < std::min(str1.length(), str2.length()); i++)
        {
            const int x = dis(rng);
            if (x == 0)
                str3 += str1[i];
            else if (x == 1)
                str3 += str2[i];
        }
        if (str3.length() < str1.length())
            str3 += str1.substr(i);
        else if (str3.length() < str2.length())
            str3 += str2.substr(i);
        return str3;
    }

    // returns 1 for the given probability
    // eg. p = 1.0 always returns 1
    int weighted_prob(const double &p)
    {
        std::uniform_real_distribution<double> dis(0.0, 1.0);
        const double x = dis(rng);
        if (x <= p)
            return 1;
        else
            return 0;
    }

    std::filesystem::path get_ecosystem_root()
    {
        return std::filesystem::canonical(__FILE__).parent_path().parent_path().parent_path();
    }

    bool is_nonzero_nonnegative_nonnan(const double &x)
    {
        if (x <= 0 || x != x)
            return false;
        return true;
    }

    std::string generate_full_species_name(const std::string &kingdom, const std::string &kind)
    {
        return kingdom + "/" + kind;
    }
    
    std::vector<uint8_t> string_to_bytevector(const std::string& str)
    {
        std::vector<uint8_t> ans;
        size_t len = str.length();
        for(size_t i = 0; i <= len / 8; i++)
        {
            if(i != len / 8 || len % 8 != 0)
                ans.push_back(to_decimal(str.substr(i * 8, (i == len / 8 ? len % 8 : 8))));
        }
        return ans;
    }
    
    std::string bytevector_to_string(const uint8_t* arr, const size_t& len)
    {
        std::string str;
        for(size_t i = 0; i < len; i++)
            str += fmt::format("{:b}", arr[i]);
        return str;
    }
    
    uint32_t get_value_from_bytearray(const uint8_t* arr, const size_t& arr_len, const size_t& start, const size_t & len)
    {
        return to_decimal(bytevector_to_string(arr, arr_len).substr(start, len));
    }
};
