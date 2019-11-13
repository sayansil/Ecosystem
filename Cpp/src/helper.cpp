#include <helper.hpp>

std::string helper::to_binary(const unsigned int& x)
{
    auto num = x; std::string str = "";
    while(num)
    {
        str = std::to_string(num % 2) + str;
        num >>= 1;
    }
    return str;
}

unsigned int helper::to_decimal(const std::string& str)
{
    unsigned int ans = 0; auto n = str.length();
    for(int i = n - 1; i >= 0; i--)
    {
        if(str[i] == '1')
            ans = ans + (1 << (n - i -1));
    }
    return ans;
}

std::string helper::random_binary(const int& inp)
{
    std::string str = "";
    std::uniform_int_distribution<int> dis(0, 1);
    std::mt19937_64 rng; rng.seed(std::random_device()());
    for(int i = 0; i < inp; i++)
        str += std::to_string(dis(rng));
    return str;
}

std::string helper::random_name(const int& inp)
{
    std::string str = "";
    std::uniform_int_distribution<int> dis(0, 35);
    std::mt19937_64 rng; rng.seed(std::random_device()());
    for(int i = 0; i < inp; i++)
    {
        int tmp = dis(rng);
        if(tmp >= 0 && tmp <= 9)
            str += std::to_string(tmp);
        else
            str += (char)(tmp - 10 + 'a');
    }
    return str;
}

std::pair<unsigned int, unsigned int> helper::random_location()
{
    std::uniform_int_distribution<unsigned int> dis_height(0, map_height - 1);
    std::uniform_int_distribution<unsigned int> dis_width(0, map_width - 1);
    std::mt19937_64 rng; rng.seed(std::random_device()());
    return {dis_width(rng), dis_height(rng)};
}

std::string helper::get_value_from_chromosome(const std::string& chromosome, const int& start, const int& length)
{
    return chromosome.substr(start, length);
}
