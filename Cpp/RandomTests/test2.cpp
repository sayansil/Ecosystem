#include <string>
#include <iostream>
#include <random>

std::string random_name(const int& inp)
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

int main()
{
    int n; std::cin >> n;
    std::cout << random_name(n) << '\n';
}
