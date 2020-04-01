#include <god.hpp>
#include <iostream>
#include <memory>

bool test_nodb()
{
    bool flag;

    try
    {
        unsigned int initial_organism_count = 200;
        unsigned int years_to_simulate = 100;

        God allah;
        allah.reset_species("plant/bamboo");

        while (initial_organism_count--)
        {
            allah.spawn_organism(std::make_shared<Plant>("bamboo", 10));
        }

        while (years_to_simulate--)
        {
            allah.happy_new_year();
        }

        allah.~God();

        flag = true;
    }
    catch(const std::exception& e)
    {
        std::cerr << "test_nodb failed." << '\n';
        std::cerr << e.what() << '\n';
        flag = false;
    }

    return flag;
}

bool test_masterdb()
{
    bool flag;

    try
    {
        unsigned int initial_organism_count = 200;
        unsigned int years_to_simulate = 100;

        God allah(true);
        allah.reset_species("plant/bamboo");

        while (initial_organism_count--)
        {
            allah.spawn_organism(std::make_shared<Plant>("bamboo", 10));
        }

        while (years_to_simulate--)
        {
            allah.happy_new_year();
        }

        allah.~God();

        DatabaseManager db;
        auto rows = db.read_all_rows_stats("plant/bamboo");

        if (rows.size() == years_to_simulate)
        {
            flag = true;
        }
        else
        {
            std::cerr << "test_statsdb failed." << '\n';
            std::cerr << "Did not write to STATS table." << '\n';
            flag = false;
        }

        db.~DatabaseManager();
    }
    catch (const std::exception &e)
    {
        std::cerr << "test_masterdb failed." << '\n';
        std::cerr << e.what() << '\n';
        flag = false;
    }

    return flag;
}

bool test_statsdb()
{
    bool flag;

    try
    {
        unsigned int initial_organism_count = 200;
        unsigned int years_to_simulate = 100;

        God allah;
        allah.reset_species("plant/bamboo");

        while (initial_organism_count--)
        {
            allah.spawn_organism(std::make_shared<Plant>("bamboo", 10));
        }

        while (years_to_simulate--)
        {
            allah.happy_new_year();
            allah.remember_species("plant/bamboo");
        }

        allah.~God();

        DatabaseManager db;
        auto rows = db.read_all_rows_stats("plant/bamboo");

        if (rows.size() == years_to_simulate)
        {
            flag = true;
        }
        else
        {
            std::cerr << "test_statsdb failed." << '\n';
            std::cerr << "Did not write to STATS table." << '\n';
            flag = false;
        }

        db.~DatabaseManager();
    }
    catch (const std::exception &e)
    {
        std::cerr << "test_statsdb failed." << '\n';
        std::cerr << e.what() << '\n';
        flag = false;
    }

    return flag;
}

int main()
{
    int total = 3;
    int passed = 0;

    if (test_nodb())
        passed++;

    if (test_masterdb())
        passed++;

    if (test_statsdb())
        passed++;

    std::cout << passed << "/" << total << " tests passed.\n";
}
