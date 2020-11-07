#include <species_constants.hpp>
#include <iostream>

int main()
{
    constants::init();
    std::cout << constants::species_constants_map.dump() << '\n';
}
