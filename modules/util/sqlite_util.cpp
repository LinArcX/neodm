#include "sqlite_util.hpp"
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <util.hpp>
using namespace std;

SqliteUtil::SqliteUtil() {}
SqliteUtil::~SqliteUtil() {}

int SqliteUtil::open_db(string db_path)
{
    int rc;
    sqlite3* db;
    bool is_successfull = false;
    this->db_path = db_path;

    rc = sqlite3_open(db_path.c_str(), &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    } else {
        fprintf(stdout, "Opened database successfully\n");
        is_successfull = true;
    }
    sqlite3_close(db);
    return is_successfull;
}

void SqliteUtil::create_items_table()
{
    int rc;
    sqlite3* db;

    /* Open database */
    rc = sqlite3_open(this->db_path.c_str(), &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    } else {
        fprintf(stdout, "Opened database successfully\n");
        char* errMsg = 0;
        std::string cts_items = "CREATE TABLE items("
                                "ID   INTEGER PRIMARY KEY AUTOINCREMENT,"
                                "url              TEXT NOT NULL,"
                                "size             INTEGER NOT NULL,"
                                "speed            INTEGER NOT NULL,"
                                "completed        INTEGER NOT NULL,"
                                "date_added       TEXT NOT NULL,"
                                "date_completed   TEXT NOT NULL);";

        rc = sqlite3_exec(db, cts_items.c_str(), 0, 0, &errMsg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL Table creation error: %s\n", errMsg);
            sqlite3_free(errMsg);
        } else {
            fprintf(stdout, "Table created successfully\n");
        }
    }
    sqlite3_close(db);
}

void SqliteUtil::create_preferences_table()
{
    int rc;
    sqlite3* db;

    /* Open database */
    rc = sqlite3_open(this->db_path.c_str(), &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    } else {
        fprintf(stdout, "Opened database successfully\n");
        char* errMsg = 0;
        std::string cts_preferences = "CREATE TABLE preferences("
                                      "ID   INTEGER PRIMARY KEY AUTOINCREMENT,"
                                      "location              TEXT NOT NULL,"
                                      "host             INTEGER NOT NULL,"
                                      "port            INTEGER NOT NULL);";

        rc = sqlite3_exec(db, cts_preferences.c_str(), 0, 0, &errMsg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL Table creation error: %s\n", errMsg);
            sqlite3_free(errMsg);
        } else {
            fprintf(stdout, "Table created successfully\n");
        }
    }
    sqlite3_close(db);
}

vector<vector<string>> SqliteUtil::get_items()
{
    int rc;
    sqlite3* db;
    vector<vector<string>> items;

    /* Open database */
    rc = sqlite3_open(this->db_path.c_str(), &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    } else {
        sqlite3_stmt* stmt;

        sqlite3_prepare_v2(db, "select * from items", -1, &stmt, NULL);
        while ((sqlite3_step(stmt)) == SQLITE_ROW) {
            vector<string> row_items;

            std::string ID = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
            std::string url = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            std::string size = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            std::string speed = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
            std::string completed = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
            std::string date_added = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
            std::string date_completed = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)));

            row_items.push_back(ID);
            row_items.push_back(url);
            row_items.push_back(size);
            row_items.push_back(speed);
            row_items.push_back(completed);
            row_items.push_back(date_added);
            row_items.push_back(date_completed);

            items.push_back(row_items);
        }
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
    return items;
}

void SqliteUtil::insert_link(string link)
{
    int rc;
    sqlite3* db;

    /* Open database */
    rc = sqlite3_open(this->db_path.c_str(), &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    } else {
        time_t now = time(0);
        char* dt = ctime(&now);

        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, "insert into items"
                               "(url, size, speed, completed, date_added, date_completed)"
                               "values (?1, ?2, ?3, ?4, ?5, ?6)",
            -1, &stmt, NULL);
        sqlite3_bind_text(stmt, 1, link.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, 0);
        sqlite3_bind_int(stmt, 3, 0);
        sqlite3_bind_int(stmt, 4, 0);
        sqlite3_bind_text(stmt, 5, dt, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 6, "---", -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            printf("ERROR inserting data: %s\n", sqlite3_errmsg(db));
            goto SHUTDOWN;
        }
        sqlite3_finalize(stmt);
    }
SHUTDOWN:
    sqlite3_close(db);
}

void SqliteUtil::update_items(std::vector<std::vector<std::string>> latest_data)
{
    int rc;
    sqlite3* db;
    char* errMsg = 0;

    /* Open database */
    rc = sqlite3_open(this->db_path.c_str(), &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    } else {
        for (int i = 0; i < latest_data.size(); i++) {
            std::string stm_container;
            stm_container = "UPDATE items set size="
                + latest_data[i][2] + ", "
                + "speed=" + latest_data[i][3] + ", "
                + "completed=" + latest_data[i][4] + " "
                + "where ID=" + latest_data[i][0];
            rc = sqlite3_exec(db, stm_container.c_str(), 0, 0, &errMsg);

            if (rc != SQLITE_OK) {
                log_message("ERROR update items table: ");
                log_message(sqlite3_errmsg(db));
                log_message("\n");
                fprintf(stderr, "Error: Update items failed!: %s\n", errMsg);
                sqlite3_free(errMsg);
                goto SHUTDOWN;
            } else {
                log_message("Update: done successfully!\n");
                fprintf(stdout, "Update: done successfully\n");
            }
        }
    }

SHUTDOWN:
    sqlite3_close(db);
}
