#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

int main()
{
    std::vector<int> arr(10); std::iota(arr.begin(), arr.end(), 0);
    std::mt19937_64 rng{std::random_device()()};
    std::shuffle(arr.begin(), arr.end(), rng);
    for(const auto& i : arr)
        std::cout << i << '\n';
}
