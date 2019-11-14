#include <filesystem>
#include <iostream>
#include <string>
#include <utility>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

int main()
{
    std::filesystem::path p = std::filesystem::current_path();
    auto logger = spdlog::stdout_color_mt("logger");
    logger->set_pattern("%^[%n] %l: %v%$");
    logger->info("Searching for ecosystem config.json in "+p.string());
    bool config_dir_found = false, config_file_found = false;
    auto func = [&](){
        if( std::filesystem::exists(p / std::filesystem::path("config")))
        {
            logger->info("config directory found");
            config_dir_found = true;
            if(std::filesystem::exists(p / "config/config.json"))
            {
                logger->info("config.json found in " + p.string() + "/config");
                config_file_found = true;
            }
            else
            {
                logger->warn("config.json not found in " + p.string() + "/config");
            }
        }
    };
    func();
    if(!config_file_found)
    {
        if(!config_dir_found)
            logger->warn("config directory not found in " + p.string());
        p = p.parent_path();
        logger->info("looking in " + p.string());
        func();
        if(!config_dir_found)
        {
            logger->warn("config directory not found in " + p.string());
            logger->critical("No config.json found. Exiting");
        }
        else if(!config_file_found)
            logger->critical("No config.json found. Exiting");
    }
}
