#include <database_manager.hpp>

int main()
{
    DatabaseManager dbm;
    auto vec = dbm.read_all_rows_master();
    std::cout << vec.size() << '\n';
}
