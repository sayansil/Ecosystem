#include <god.hpp>
#include <iostream>
#include <fmt/core.h>
#include <vector>
#include <unordered_map>

int main()
{
    fmt::print("Hello world\n");
    unsigned int initial_organism_count = 45000;
    std::vector<std::unordered_map<std::string, std::string>> organisms;
    organisms.reserve(initial_organism_count);

    for(size_t i = 0; i < initial_organism_count; i++)
    {
        organisms.push_back({{"kind", "deer"},
                {"kingdom", "0"},
                {"age", "20"}
                });
    }
    
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
    fmt::print("Buffer size = {}\n", allah.buffer.size());
}
