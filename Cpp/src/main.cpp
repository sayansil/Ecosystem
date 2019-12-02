#include <herbivore.hpp>
#include <iostream>
#include <helper.hpp>
#include <arrayfire.h>

int main()
{
    Deer::initialize();
    std::vector<float> X;
    std::vector<float> Y;
    int k = 1;
    Deer obj("10001111000010010011110001010010101101111000011010001000111011000011110100000101110011010111111111010011000000001110010011010101011", 1, "Ayon");
    for(int i = 0; i < 50; i++)
    {
        auto stats = obj.get_stats();
//        for(const auto& [j, k] : stats)
//            std::cout << j << ", " << k << '\n';
        Y.push_back(std::stof(stats["death_factor"]));
        X.push_back(k++);
        obj.increment_age();
    }
    X.shrink_to_fit();
    Y.shrink_to_fit();
    af::array x(X.size(), X.data());
    af::array y(Y.size(), Y.data());
    af::Window win("Plot");
    while(true)
    {
        win.plot(x, y);
        win.show();
    }
}
