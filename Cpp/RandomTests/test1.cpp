#include <god.hpp>
#include <iostream>
#include <helper.hpp>
#include <stat_fetcher.hpp>
#include <vector>
#include <algorithm>
int main()
{
    unsigned int initial_organism_count = 100;
    unsigned int years_to_simulate = 10;

    God allah(true);
    allah.reset_species("plant/bamboo");

    while (initial_organism_count--)
    {
        allah.spawn_organism(std::make_shared<Plant>("bamboo", 10));
    }

//    std::vector<std::vector<std::string>> data;
//    std::vector<std::string> avg_data;
//    std::vector<PStat> temp;
//    std::vector<std::string> temp2;
    int k = 0;

    while (years_to_simulate--)
    {
        allah.happy_new_year(true);
        allah.remember_species("plant/bamboo");

        int l, h;
        std::tie(l, h) = stat_fetcher::get_stat_gap(allah.organisms, "age", "bamboo");
        std::cout << "Low: " << l << ", High: " << h << "\n";
//        avg_data.push_back(std::to_string(stat_fetcher::get_stat_average(allah.organisms, "age", "bamboo")));
//        temp = stat_fetcher::get_one_stat(allah.organisms, "age", "bamboo");
//        std::transform(temp.begin(), temp.end(), std::back_inserter(temp2), [](PStat p){return p.getString();});
//        data.push_back(temp2);
//        temp.clear();
//        temp2.clear();
    }

//    for (auto temp: data)
//    {
//        std::cout << "Average: " << avg_data[k] << ", after " << k++ << " years:\n";
//        for (auto t: temp)
//        {
//            std::cout << t << ", ";
//        }
//        std::cout << "\n\n";
//    }
}
