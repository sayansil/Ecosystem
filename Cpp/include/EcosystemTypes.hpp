#ifndef ECOSYSTEMTYPES_HPP
#define ECOSYSTEMTYPES_HPP

#include <variant>
#include <map>

using stat_type = std::variant<unsigned int, double, std::string, bool>;
using chromosome_map_type = std::map<std::string, std::map<std::string, unsigned int>>;

#endif // ECOSYSTEMTYPES_HPP
