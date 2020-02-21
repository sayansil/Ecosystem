#ifndef ECOSYSTEMTYPES_HPP
#define ECOSYSTEMTYPES_HPP

#include <map>
#include <variant>


using CHROMOSOME_MAP_TYPE = std::map<std::string, std::map<std::string, unsigned int>>;
using STAT_TYPE           = std::variant<unsigned int, double, std::string, bool>;

#endif // ECOSYSTEMTYPES_HPP
