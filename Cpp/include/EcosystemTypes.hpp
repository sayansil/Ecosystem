#ifndef ECOSYSTEMTYPES_HPP
#define ECOSYSTEMTYPES_HPP

#include <map>
#include <variant>

using chromosome_map_type = std::map<std::string, std::map<std::string, unsigned int>>;
using stat_type           = std::variant<unsigned int, double, std::string, bool>;

#endif // ECOSYSTEMTYPES_HPP
