#ifndef SETUP_HPP
#define SETUP_HPP

#include <filesystem>
#include <iostream>
#include <fstream>
#include <sqlite3.h>
#include <string>
#include <helper.hpp>

namespace setup
{
    extern std::filesystem::path setup();
};

#endif /* SETUP_HPP */
