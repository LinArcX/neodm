#include "launcher.hpp"

#include <memory>
#include <stdio.h>
#include <sys/stat.h>

#include <sw_about.hpp>
#include <sw_active.hpp>
#include <sw_key_bindings.hpp>
#include <sw_new.hpp>
#include <util.hpp>

using namespace std;

Launcher::Launcher()
{
    _sqlite_util = std::make_shared<SqliteUtil>();

    string home_name = "/home/";
    char* user_name = getenv("USER");
    string parent_directory = "/.config/neodm";
    string full_directory = "/.config/neodm/neodm.db";

    db_path = home_name + string(user_name) + parent_directory;
    db_full_path = home_name + string(user_name) + full_directory;

    FILE* fp;
    if ((fp = fopen(db_full_path.c_str(), "r")) == NULL) {
        struct stat st = { 0 };
        if (stat(db_path.c_str(), &st) == -1) {
            mkdir(db_path.c_str(), 0700);
        }
    }

    int db_status = _sqlite_util->open_db(db_full_path);
    if (db_status) {
        _sqlite_util->create_items_table();
    } else {
        delwin(sw_content);
        delwin(sw_footer);
        endwin();
        exit(0);
    }
}

Launcher::~Launcher()
{
    delwin(sw_content);
    delwin(sw_footer);
    endwin();
}

int Launcher::init_ncurses()
{
    /* Initialize curses */
    screen = initscr();
    cbreak();
    noecho();

    /* Get screen size */
    getmaxyx(screen, y_max, x_max);

    /*  Check if the terminal can manipulate colors; */
    if (!has_colors()) {
        printw("Your terminal doesn't support colors!\n");
        getch();
        return EXIT_FAILURE;
    } else {
        start_color();
        pair_green_black = init_pair(1, COLOR_GREEN, COLOR_BLACK);
        pair_white_black = init_pair(2, COLOR_WHITE, COLOR_BLACK);
        pair_red_black = init_pair(3, COLOR_RED, COLOR_BLACK);
        pair_cyan_black = init_pair(4, COLOR_CYAN, COLOR_BLACK);
    }

    /* Enable keypad and start active_window*/
    keypad(screen, TRUE);

    /* make the cursor invisible */
    curs_set(0);
    return EXIT_SUCCESS;
}

void Launcher::update_window(WINDOW* window)
{
    touchwin(window);
    wrefresh(window);
}

void Launcher::process_input()
{
    std::shared_ptr<SwAbout> _sw_about = std::make_shared<SwAbout>(shared_from_this());
    std::shared_ptr<SwActive> _sw_active = std::make_shared<SwActive>(shared_from_this());
    std::shared_ptr<SwNew> _sw_new = std::make_shared<SwNew>(shared_from_this());
    std::shared_ptr<SwKeyBindings> _sw_key_bindings = std::make_shared<SwKeyBindings>(shared_from_this());

    int c = 0, is_first = 1;
    static int old = 0;
    c = _sw_active->show_active_window();
    //goto chooser;

loop:
    if (is_first == 1) {
        old = KEY_F(2);
        is_first = 0;
    } else {
        c = wgetch(screen);
    }
chooser:
    switch (c) {
    case KEY_F(1):
        old = c;
        c = _sw_about->show_about_window();
        goto chooser;
    case KEY_F(2):
        old = c;
        c = _sw_active->show_active_window();
        goto chooser;
    case KEY_F(4):
        old = c;
        c = _sw_key_bindings->show_window();
        goto chooser;
    case 'n':
        _sw_new->show_new_window(sw_content);
        c = old;
        goto chooser;
    case 'q':
        break;
    default:
        goto loop;
    }
}

void Launcher::show_content_subwin(WINDOW* screen)
{
    sw_content = subwin(screen, y_max - 3, x_max, 0, 0);
    wborder(sw_content, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(sw_content);
}

void Launcher::show_footer_subwin(WINDOW* screen)
{
    sw_footer = subwin(screen, 3, x_max, y_max - 3, 0);
    wborder(sw_footer, 0, 0, 0, 0, 0, 0, 0, 0);
}

void Launcher::draw_sub_windows()
{
    /* Split screen into two subwindow and update screen */
    show_content_subwin(screen);
    show_footer_subwin(screen);
}
