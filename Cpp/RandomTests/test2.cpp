#include <database_manager.hpp>

int main()
{
    DatabaseManager dbm;
    dbm = DatabaseManager();
    auto vec = dbm.read_all_rows_from("ECOSYSTEM_MASTER");

    std::cout << vec.size() << '\n';
}