#ifndef SQLITE_UTIL_H
#define SQLITE_UTIL_H

#include <sqlite3.h>
#include <string>
#include <vector>

class SqliteUtil {
private:
    std::string db_path;

public:
    SqliteUtil();
    virtual ~SqliteUtil();

    int open_db(std::string db_path);
    void create_items_table();
    std::vector<std::vector<std::string>> get_items();
    void insert_link(std::string link);
    void update_items(std::vector<std::vector<std::string>> latest_data);
};

#endif /* SQLITE_UTIL_H */
