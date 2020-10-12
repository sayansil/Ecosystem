#ifndef SCHEMA_HPP
#define SCHEMA_HPP

#include <vector>
#include <ecosystem_types.hpp>

namespace schema
{
    extern const std::vector<std::pair<std::string, SQLType>> schemaMaster;
    extern const std::vector<std::pair<std::string, SQLType>> schemaAnimal;
    extern const std::vector<std::pair<std::string, SQLType>> schemaPlant;
}; // namespace schema

#endif // SCHEMA_HPP
