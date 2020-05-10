#ifndef Launcher_H
#define Launcher_H

#include <iostream>
#include <memory>
#include <ncurses.h>

#include <sqlite_util.hpp>

class Launcher : public std::enable_shared_from_this<Launcher> {
private:
    std::shared_ptr<SqliteUtil> _sqlite_util;
    std::string db_path;
    std::string db_full_path;

    std::string app_version = "neodm 1.0.0";
    std::string github_link = "github.com/LinArcX/neodm";

    int y_max;
    int x_max;
    int pair_green_black = 0;
    int pair_white_black = 0;
    int pair_red_black = 0;
    int pair_cyan_black = 0;

    WINDOW* screen;
    WINDOW* sw_content;
    WINDOW* sw_footer;

    const std::string m_file = "File";
    const std::string m_edit = "Edit";
    const std::string m_view = "View";
    const std::string m_help = "Help";

    const int m_items_pos_y = 1;
    const int m_file_pos_x = 2;
    const int m_edit_pos_x = 8;
    const int m_view_pos_x = 14;
    const int m_help_pos_x = 20;

public:
    Launcher();
    virtual ~Launcher();

    int init_ncurses();

    void show_content_subwin(WINDOW* screen);
    void show_footer_subwin(WINDOW* screen);
    void draw_sub_windows();

    void update_window(WINDOW* window);
    void process_input();

    // getters
    int get_y_max() { return y_max; }
    int get_x_max() { return x_max; }

    int get_items_pos_y() { return m_items_pos_y; }
    int get_files_pos_x() { return m_file_pos_x; }
    int get_edit_pos_x() { return m_edit_pos_x; }
    int get_view_pos_x() { return m_view_pos_x; }
    int get_help_pos_x() { return m_help_pos_x; }

    std::string get_txt_file() { return m_file; }
    std::string get_txt_edit() { return m_edit; }
    std::string get_txt_view() { return m_view; }
    std::string get_txt_help() { return m_help; }

    WINDOW* get_screen() { return screen; }
    WINDOW* get_sw_content() { return sw_content; }
    WINDOW* get_sw_footer() { return sw_footer; }

    std::shared_ptr<SqliteUtil> get_sqlite_util() { return _sqlite_util; }

    std::string get_db_full_path() { return db_full_path; }
    std::string get_app_name() { return app_version; }
    std::string get_github_link() { return github_link; }
};

#endif /* Launcher_H */
