#include <god.hpp>
#include <iostream>

int main()
{
    std::vector<std::string> vars {"height", "weight"};
    God allah;
    allah.reset_species("animal/deer");

    allah.test_organism(std::make_shared<Animal>("deer", 10), vars, 20);
}
