#include <schema.hpp>

namespace schema
{
    extern DLLEXPORT const std::vector<std::pair<std::string, std::string>> schemaMaster{
        {"YEAR", "NUMBER"},
        {"AVG_WORLD", "LONGBLOB"},
        {"POPULATION_WORLD", "LONGBLOB"}};
}; // namespace schema
