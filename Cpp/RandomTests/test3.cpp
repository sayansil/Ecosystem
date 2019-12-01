#include <herbivore.hpp>
#include <iostream>
#include <helper.hpp>

int main()
{
    Deer::initialize();
    Deer obj(Deer::generate_random_chromosome(), 1, "Ayon");
    for(int i = 0; i < 10; i++)
    {
        auto stats = obj.get_stats();
        for(const auto& [j, k] : stats)
            std::cout << j << ", " << k << '\n';
        obj.increment_age();
    }
}
