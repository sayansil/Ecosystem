#include <vector>
#include <sqlite3.h>
#include <animal.hpp>

int insertRows(const std::vector<std::vector<stat_type>>& rows)
{
    for(const auto& row : rows)
    {
        std::string values = "";
        for(const auto& value : row)
        {
            if(value.index() == 0)
            {
                values += std::to_string(std::get<unsigned int>(value)) + ",";
            }
            if(value.index() == 1)
            {
                values += std::to_string(std::get<double>(value)) + ",";
            }
            if(value.index() == 2)
            {
                values += "\'" + std::get<std::string>(value) + "\',";
            }
            if(value.index() == 3)
            {
                bool tmp = std::get<bool>(value);
                if(tmp)
                    values += "\'true\',";
                else
                    values += "\'false\',";
            }
        }
        values = values.substr(0, values.length() - 1);
        std::string sql_command = "INSERT INTO ECOSYSTEM_MASTER (NAME,KIND,CHROMOSOME,GENERATION,IMMUNITY,GENDER,AGE,HEIGHT,WEIGHT,FITNESS) "\
                                  "VALUES (" + values + ");";
        std::cout << sql_command << '\n';
    }
    return 0;
}

int main()
{
    std::vector<stat_type> arr;

    arr.push_back(stat_type(std::string("lion123")));
    arr.push_back(stat_type(std::string("lion")));
    arr.push_back(stat_type((unsigned int)10101));
    arr.push_back(stat_type((unsigned int)2));
    arr.push_back(stat_type(4.5));
    arr.push_back(stat_type((unsigned int)15));
    arr.push_back(stat_type(5.6));
    arr.push_back(stat_type(6.6));
    arr.push_back(stat_type(7.6));

    std::vector<std::vector<stat_type>> arr1;
    arr1.push_back(arr);
    insertRows(arr1);
}
