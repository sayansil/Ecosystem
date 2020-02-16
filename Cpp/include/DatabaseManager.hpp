#ifndef DATABASEMANAGER_HPP
#define DATABASEMANAGER_HPP

#include <sqlite3.h>
#include <iostream>
#include <filesystem>
#include <animal.hpp>
#include <vector>
#include <string>
#include <unordered_map>

struct DatabaseManager
{
    DatabaseManager();
    DatabaseManager(const std::filesystem::path&);
    void insertRows(const std::vector<std::vector<stat_type>>&);
    void deleteRows(const std::vector<std::string>&);
    std::vector<std::vector<stat_type>> readRows(const std::string&, const std::vector<std::string>&);
    void updateRows(const std::vector<std::vector<stat_type>>&);
    std::unordered_map<std::string, std::vector<std::vector<stat_type>>> groupByKind();
    void clearDatabase();
    ~DatabaseManager();

    sqlite3 *db;
    std::filesystem::path db_path;
};

#endif // DATABASEMANAGER_HPP
