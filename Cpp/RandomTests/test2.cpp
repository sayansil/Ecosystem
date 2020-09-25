#include <god.hpp>
#include <iostream>
#include <helper.hpp>
#include <map_maker.hpp>

int main()
{
    auto animal = std::make_shared<Animal>("deer");
    God allah(true);
    allah.init(*animal);
    ENTITY entity = animal;
    auto &map2 = entity->get_attribute_raw_map();
    map2.map["height"].assign("0.6969");
    for(const auto& [i, j] : animal->get_attribute_raw_map().map)
        std::cout << i << ": " << j.getString() << '\n';
}
