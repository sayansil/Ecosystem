#include <filesystem>
#include <iostream>
#include <helper.hpp>
#include <stat_fetcher.hpp>

int main()
{

        std::filesystem::path species_folder = "../../data/json";

        for(auto file : std::filesystem::directory_iterator(species_folder))
        {
            std::string current_species = file.path();
            current_species = current_species.substr(current_species.find_last_of('/') + 1);
            const std::string filepath = file / std::filesystem::path("current.json");
            std::cout << current_species << '\n' << filepath << "\n****\n"; 
        }
}
