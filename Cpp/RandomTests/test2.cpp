#include <god.hpp>
#include <iostream>
#include <fmt/core.h>
#include <vector>
#include <unordered_map>

int main()
{
    fmt::print("Hello world\n");
    unsigned int initial_organism_count = 200;
    std::vector<std::unordered_map<std::string, std::string>> organisms;

    organisms.push_back({{"kind", "deer"},
                         {"kingdom", "animal"},
                         {"age", "20"},
                         {"name", "sreenik"},
                         {"chromosome", ""},
                         {"generation", "1"},
                         {"X", "0"},
                         {"Y", "0"},
                         {"monitor", "0"}});
    // organisms.push_back({{"kingdom", "animal"},
    //                      {"kind", "deer"},
    //                      {"name", "deer2-rmg"}});
    // organisms.push_back({{"kingdom", "plant"},
    //                      {"kind", "bamboo"},
    //                      {"name", "bamboo1-shanti"}});
    // organisms.push_back({{"kingdom", "plant"},
    //                      {"kind", "bamboo"},
    //                      {"name", "bamboo2-sayan"}});

    God allah;

    allah.createWorld(organisms);
    allah.displayWorldMetadata();
}
