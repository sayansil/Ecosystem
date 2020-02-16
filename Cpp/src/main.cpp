#include <iostream>
#include <helper.hpp>

int main()
{
    const double mutationChance = 0.5;
    std::string child_chromosome = "1010101010101010";
    std::cout << child_chromosome << '\n';
    for(auto& bit : child_chromosome)
        if(helper::weighted_prob(mutationChance))
            bit = (bit == '1')?'0':'1';
    std::cout << child_chromosome << '\n';
}
