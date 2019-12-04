#include <variant>
#include <iostream>
#include <map>

struct Visitor
{
    void operator()(int& in)
    {
        std::cout << "Integer\n";
    }
    void operator()(std::string& in)
    {
        std::cout << "String\n";
    }
    void operator()(double& in)
    {
        std::cout << "Double\n";
    }
};

int main()
{
    std::map<std::string, std::variant<std::string, int, double>> x;
    x["a"] = 12;
    x["b"] = "12";
    x["c"] = 123.345;
    for(auto it = x.begin(); it != x.end(); it++)
    {
        std::cout << it->first << ' ';
        std::visit(Visitor{}, it->second);
        std::cout << it->second.index() << '\n';
    }
    std::cout << (int)std::get<double>(x["c"]) << '\n';
}
