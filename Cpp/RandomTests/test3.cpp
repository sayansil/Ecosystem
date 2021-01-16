#include <god.hpp>
#include <iostream>

int main()
{
    constants::init();
    ENTITY obj = std::make_shared<Animal>("deer", 10);
    const auto& a_map = stat_fetcher::get_var_map(obj);
    std::cout << a_map.is_initialized << ' ' << a_map.map.size() << '\n';
    const auto& a_map2 = stat_fetcher::get_var_map(obj);
    std::cout << a_map2.is_initialized << ' ' << a_map2.map.size() << '\n';
}
