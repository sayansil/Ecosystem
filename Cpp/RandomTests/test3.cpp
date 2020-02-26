#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>

static void attach_missing_json_files()
{
    const std::filesystem::path json_data_path = "../../data/json";
    std::filesystem::path filepath;

    for (const auto &entry : std::filesystem::directory_iterator(json_data_path))
    {
        filepath = "base.json";
        if (!std::filesystem::exists(entry.path() / filepath))
        {
            std::ofstream to_create(entry.path() / filepath);
            to_create << "{}";
            to_create.close();
        }

        filepath = "current.json";
        if (!std::filesystem::exists(entry.path() / filepath))
        {
            std::ofstream to_create(entry.path() / filepath);

            std::ifstream to_copy(entry.path() / "base.json");
            std::string tmp((std::istreambuf_iterator<char>(to_copy)),
                            std::istreambuf_iterator<char>());
            to_create << tmp;

            to_copy.close();
            to_create.close();
        }

        filepath = "modify.json";
        if (!std::filesystem::exists(entry.path() / filepath))
        {
            std::ofstream to_create(entry.path() / filepath);
            to_create << "{}";
            to_create.close();
        }
    }
}

int main()
{
    attach_missing_json_files();
}
