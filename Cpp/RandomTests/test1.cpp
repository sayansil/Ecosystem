#include "nlohmann/json_fwd.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <vector>

int main()
{
    nlohmann::json data;
    data["pi"] = 3.14;
    data["list"] = {1, 5, 2, 4, 3};
    std::cout << data << '\n';
}
