#include <DatabaseManager.hpp>
#include <iostream>
#include <vector>
#include <helper.hpp>

void insert()
{
    DatabaseManager obj;
    std::vector<std::vector<stat_type>> rows;
    rows.push_back(std::vector<stat_type>({
                std::string("Lion123"),
                std::string("lion"),
                std::string("01010101010101010101010010101000"),
                (unsigned int)50,
                14.5,
                (unsigned int)0,
                (unsigned int)15,
                67.4,
                34.7,
                23.9
    }));
    rows.push_back(std::vector<stat_type>({
                std::string("Lion321"),
                std::string("lion"),
                std::string("010101010101010"),
                (unsigned int)5,
                1.145,
                (unsigned int)1,
                (unsigned int)145,
                674.1,
                3.47,
                2.39
    }));
    rows.push_back(std::vector<stat_type>({
                std::string("Deer123"),
                std::string("deer"),
                std::string("01010101010101010101010010101000100000000110111"),
                (unsigned int)50,
                14.5,
                (unsigned int)0,
                (unsigned int)15,
                607.4,
                134.7,
                213.93
    }));
    obj.insertRows(rows);
}

void update()
{
    DatabaseManager obj;
    std::vector<std::vector<stat_type>> rows;
    rows.push_back(std::vector<stat_type>({
                std::string("Lion123"),
                std::string("lion"),
                std::string("01010101010101010101010010101000"),
                (unsigned int)6,
                14.5,
                (unsigned int)0,
                (unsigned int)15,
                67.4,
                34.7,
                23.9
    }));
    rows.push_back(std::vector<stat_type>({
                std::string("Deer123"),
                std::string("deer"),
                std::string("01010101010101010101010010101000100000000110111"),
                (unsigned int)600,
                14.5,
                (unsigned int)0,
                (unsigned int)15,
                607.4,
                134.7,
                213.93
    }));
    obj.updateRows(rows);
}

void read()
{
    DatabaseManager obj;
    std::vector<std::vector<stat_type>> rows = obj.readRows("NAME", {"Lion123", "Lion321"});
    for(const auto& row : rows)
    {
        for(const auto& item : row)
            helper::display_stat(item);
        std::cout << '\n';
    }
}

void groupByKind()
{
    DatabaseManager obj;
    auto rowMap = obj.groupByKind();
    for(const auto& [kind, rows] : rowMap)
    {
        std::cout << "KIND: " << kind << ":\n";
        for(const auto& row : rows)
        {
            for(const auto& item : row)
                helper::display_stat(item);
            std::cout << "\n";
        }
    }
}

void del()
{
    DatabaseManager obj;
    obj.deleteRows({"Lion321"});
}

int main()
{
    DatabaseManager obj;
    obj.clearDatabase();
    insert();
    groupByKind();
}
