#include "sw_active.hpp"
#include "extern.hpp"
#include <aria2/aria2.h>
#include <chrono>
#include <cmath>
#include <curses.h>
#include <iostream>
#include <menu.h>
#include <mutex>
#include <panel.h>
#include <stdlib.h>
#include <thread>
#include <util.hpp>

using namespace std;

int downloadEventCallback(aria2::Session* session, aria2::DownloadEvent event,
    aria2::A2Gid gid, void* userData)
{
    SwActive* sa = static_cast<SwActive*>(userData);

    switch (event) {
    case aria2::EVENT_ON_DOWNLOAD_COMPLETE:
        initial_data[index][4] = initial_data[index][2];
        //sa->drawDynamicItems();
        //log_message("COMPLETE");
        break;
    case aria2::EVENT_ON_DOWNLOAD_ERROR:
        //log_message("ERROR");
        break;
    default:
        return 0;
    }
    return 0;
}

int downloaderJob(SwActive* form,
    std::vector<std::string> uris,
    aria2::KeyVals options)
{
    // session is actually singleton: 1 session per process
    aria2::Session* session;
    // Use default configuration
    aria2::SessionConfig config;

    config.downloadEventCallback = downloadEventCallback;
    config.keepRunning = true;

    session = aria2::sessionNew(aria2::KeyVals(), config);
    aria2::addUri(session, 0, uris, options);

    auto start = std::chrono::steady_clock::now();
    int previous_downloaded_data = 0;
    bool is_first = true;
    float rate_factor = 1.0;

    for (;;) {
        int rv = aria2::run(session, aria2::RUN_ONCE);
        if (rv != 1) {
            break;
        }
        auto now = std::chrono::steady_clock::now();
        auto count = std::chrono::duration_cast<std::chrono::milliseconds>(now - start)
                         .count();
        if (count >= 900) {
            start = now;
            std::vector<aria2::A2Gid> gids = aria2::getActiveDownload(session);
            for (auto gid : gids) {
                aria2::DownloadHandle* dh = aria2::getDownloadHandle(session, gid);
                if (dh) {
                    config.userData = dynamic_cast<void*>(form);

                    std::lock_guard<std::mutex> l(_mutex);
                    if (initial_data[index][2] == "0") {
                        initial_data[index][2] = std::to_string(dh->getTotalLength() / (1024 * 1024));
                    }

                    initial_data[index][3] = std::to_string(dh->getDownloadSpeed() / 1024);

                    if (is_first) {
                        previous_downloaded_data = std::stoi(initial_data[index][4]);
                        if (std::stoi(initial_data[index][2]) != 0) {
                            int reminded = (std::stoi(initial_data[index][2])) - previous_downloaded_data;
                            rate_factor = (float)reminded / (float)(std::stoi(initial_data[index][2]));
                        }
                        is_first = false;
                    }

                    initial_data[index][4] = std::to_string(previous_downloaded_data
                        + (int)(rate_factor * (dh->getCompletedLength() / (1024 * 1024))));

                    form->drawDynamicItems();
                    aria2::deleteDownloadHandle(dh);
                }
            }
        }
    }
    int rv = aria2::sessionFinal(session);
    return rv;
}

SwActive::SwActive(std::shared_ptr<Launcher> launcher)
    : _launcher(launcher)
{
    aria2::libraryInit();
    x_first_col = 0;
    x_second_col = x_first_col + ((_launcher->get_x_max() / 14) * 0.5);
    x_third_col = x_second_col + ((_launcher->get_x_max() / 14) * 9.5);
    x_forth_col = x_third_col + ((_launcher->get_x_max() / 14) * 1.3);
    x_fifth_col = x_forth_col + ((_launcher->get_x_max() / 14) * 1.3);
    x_sixth_col = x_fifth_col + ((_launcher->get_x_max() / 14) * 1.4);
    x_seventh_col = x_sixth_col + ((_launcher->get_x_max() / 14) * 1);
}

SwActive::~SwActive()
{
    for (auto& entry : threads)
        entry.join();

    aria2::libraryDeinit();
}

void SwActive::show_popup(std::string val)
{
    int items_count, i;
    int lines = 10;
    int cols = 40;

    ITEM** active_items;
    MENU* active_property_menu;

    WINDOW* active_property_win;
    PANEL* active_property_panel;

    vector<std::string> choices;
    choices.push_back(val);
    choices.push_back("*");
    choices.push_back("**");
    choices.push_back("***");

    /* Create items */
    items_count = choices.size();
    active_items = (ITEM**)calloc(items_count, sizeof(ITEM*));
    for (i = 0; i < items_count; i++)
        active_items[i] = new_item(choices[i].c_str(), choices[i].c_str());

    /* Crate menu */
    active_property_menu = new_menu((ITEM**)active_items);

    /* Set menu option not to show the description */
    menu_opts_off(active_property_menu, O_SHOWDESC);

    /* Create the window to be associated with the menu */
    active_property_win = newwin(lines, cols, _launcher->get_y_max() / 2 - lines / 2,
        _launcher->get_x_max() / 2 - cols / 2);
    box(active_property_win, 0, 0);
    keypad(active_property_win, TRUE);

    /* Set main window and sub window */
    set_menu_win(active_property_menu, active_property_win);
    set_menu_sub(active_property_menu, derwin(active_property_win, 6, 38, 3, 1));

    /* Set menu mark to the string " * " */
    set_menu_mark(active_property_menu, " * ");
    active_property_panel = new_panel(active_property_win);

    wrefresh(active_property_win);
    /* Post the menu */
    post_menu(active_property_menu);
    update_panels();
    doupdate();
    refresh();

    int c;
loop:
    c = wgetch(active_property_win);
    switch (c) {
    case 'j':
    case KEY_DOWN:
        menu_driver(active_property_menu, REQ_DOWN_ITEM);
        wrefresh(active_property_win);
        goto loop;
    case 'k':
    case KEY_UP:
        menu_driver(active_property_menu, REQ_UP_ITEM);
        wrefresh(active_property_win);
        goto loop;
    case 'q':
        break;
    }

    /* Unpost and free all the memory taken up */
    unpost_menu(active_property_menu);
    free_menu(active_property_menu);
    for (i = 0; i < items_count; i++)
        free_item(active_items[i]);

    endwin();
}

void SwActive::drawStaticItems()
{
    const string h_ID = "ID";
    const string h_url = "Url";
    const string h_size = "Size(MB)";
    const string h_speed = "Speed(KB/s)";
    const string h_complete = "Complete(MB)";
    const string h_percent = "Percent";

    col_third_length = x_third_col - x_second_col;
    col_forth_length = x_forth_col - (x_second_col + col_third_length);
    col_fifth_length = x_fifth_col - (x_second_col + col_third_length + col_forth_length);
    col_sixth_length = x_sixth_col - (x_second_col + col_third_length + col_forth_length + col_fifth_length);
    col_seventh_length = x_seventh_col - (x_second_col + col_third_length + col_forth_length + col_fifth_length + col_sixth_length);

    // line underneath the title
    mvwhline(_launcher->get_sw_content(), 0, 1, 0, _launcher->get_x_max() - 2);
    mvwaddch(_launcher->get_sw_content(), 0, x_first_col, ACS_ULCORNER);
    mvwaddch(_launcher->get_sw_content(), 1, x_first_col, ACS_VLINE);

    mvwaddch(_launcher->get_sw_content(), 0, x_second_col, ACS_TTEE);
    mvwaddch(_launcher->get_sw_content(), 0, x_third_col, ACS_TTEE);
    mvwaddch(_launcher->get_sw_content(), 0, x_forth_col, ACS_TTEE);
    mvwaddch(_launcher->get_sw_content(), 0, x_fifth_col, ACS_TTEE);
    mvwaddch(_launcher->get_sw_content(), 0, x_sixth_col, ACS_TTEE);
    mvwaddch(_launcher->get_sw_content(), 1, x_seventh_col, ACS_VLINE);
    mvwaddch(_launcher->get_sw_content(), 0, x_seventh_col, ACS_URCORNER);

    mvwprintw(_launcher->get_sw_content(),
        1, x_second_col / 2,
        "%s", h_ID.c_str());

    mvwprintw(_launcher->get_sw_content(),
        1, x_third_col / 2,
        "%s", h_url.c_str());

    mvwprintw(_launcher->get_sw_content(),
        1, x_third_col + col_forth_length / 2 - h_size.size() / 2,
        "%s", h_size.c_str());

    mvwprintw(_launcher->get_sw_content(),
        1, x_forth_col + col_fifth_length / 2 - h_speed.size() / 2,
        "%s", h_speed.c_str());

    mvwprintw(_launcher->get_sw_content(),
        1, x_fifth_col + col_sixth_length / 2 - h_complete.size() / 2,
        "%s", h_complete.c_str());

    mvwprintw(_launcher->get_sw_content(),
        1, x_sixth_col + col_seventh_length / 2 - h_percent.size() / 2,
        "%s", h_percent.c_str());

    // separators between the headers
    mvwaddch(_launcher->get_sw_content(), 1, x_second_col, ACS_VLINE);
    mvwaddch(_launcher->get_sw_content(), 1, x_third_col, ACS_VLINE);
    mvwaddch(_launcher->get_sw_content(), 1, x_forth_col, ACS_VLINE);
    mvwaddch(_launcher->get_sw_content(), 1, x_fifth_col, ACS_VLINE);
    mvwaddch(_launcher->get_sw_content(), 1, x_sixth_col, ACS_VLINE);

    // line underneath the headers
    mvwhline(_launcher->get_sw_content(), 2, 1, 0, _launcher->get_x_max() - 2);
    mvwhline(_launcher->get_sw_content(), 2, 1, 0, _launcher->get_x_max() - 2);
    mvwaddch(_launcher->get_sw_content(), 2, x_first_col, ACS_LTEE);
    mvwaddch(_launcher->get_sw_content(), 2, x_second_col, ACS_PLUS);
    mvwaddch(_launcher->get_sw_content(), 2, x_third_col, ACS_PLUS);
    mvwaddch(_launcher->get_sw_content(), 2, x_forth_col, ACS_PLUS);
    mvwaddch(_launcher->get_sw_content(), 2, x_fifth_col, ACS_PLUS);
    mvwaddch(_launcher->get_sw_content(), 2, x_sixth_col, ACS_PLUS);
    mvwaddch(_launcher->get_sw_content(), 2, x_seventh_col, ACS_RTEE);

    // draw the column separators
    for (int i = 0; i < _launcher->get_y_max() - 7; i++) {
        mvwaddch(_launcher->get_sw_content(), START_Y + i, x_first_col, ACS_VLINE);
        mvwaddch(_launcher->get_sw_content(), START_Y + i, x_second_col, ACS_VLINE);
        mvwaddch(_launcher->get_sw_content(), START_Y + i, x_third_col, ACS_VLINE);
        mvwaddch(_launcher->get_sw_content(), START_Y + i, x_forth_col, ACS_VLINE);
        mvwaddch(_launcher->get_sw_content(), START_Y + i, x_fifth_col, ACS_VLINE);
        mvwaddch(_launcher->get_sw_content(), START_Y + i, x_sixth_col, ACS_VLINE);
        mvwaddch(_launcher->get_sw_content(), START_Y + i, x_seventh_col, ACS_VLINE);
    }

    // line above the status text
    mvwhline(_launcher->get_sw_content(), _launcher->get_y_max() - 4, 1, 0, _launcher->get_x_max() - 2);
    mvwhline(_launcher->get_sw_content(), _launcher->get_y_max() - 4, 1, 0, _launcher->get_x_max() - 2);
    mvwaddch(_launcher->get_sw_content(), _launcher->get_y_max() - 4, x_first_col, ACS_LLCORNER);
    mvwaddch(_launcher->get_sw_content(), _launcher->get_y_max() - 4, x_second_col, ACS_BTEE);
    mvwaddch(_launcher->get_sw_content(), _launcher->get_y_max() - 4, x_third_col, ACS_BTEE);
    mvwaddch(_launcher->get_sw_content(), _launcher->get_y_max() - 4, x_forth_col, ACS_BTEE);
    mvwaddch(_launcher->get_sw_content(), _launcher->get_y_max() - 4, x_fifth_col, ACS_BTEE);
    mvwaddch(_launcher->get_sw_content(), _launcher->get_y_max() - 4, x_sixth_col, ACS_BTEE);
    mvwaddch(_launcher->get_sw_content(), _launcher->get_y_max() - 4, x_seventh_col, ACS_LRCORNER);
}

void SwActive::drawDynamicItems()
{
    if (!initial_data.empty()) {
        int threshold = 0;
        if (initial_data.size() <= PAGE_SIZE) {
            threshold = initial_data.size();
        } else {
            threshold = PAGE_SIZE;
        }
        for (int i = 0; i < threshold; i++) {
            _launcher->update_window(_launcher->get_sw_content());
            if (m_current_line == i)
                wattron(_launcher->get_sw_content(), A_BOLD | COLOR_PAIR(1));

            mvwprintw(_launcher->get_sw_content(),
                START_Y + i,
                x_first_col + x_second_col / 2 - initial_data[i][0].size() / 2,
                "%s",
                initial_data[i][0].c_str());

            int name_size = initial_data[i][1].size();
            mvwprintw(_launcher->get_sw_content(),
                START_Y + i,
                x_second_col + 3,
                "%s",
                name_size > x_second_col ? initial_data[i][1].substr(0, x_third_col - 10).c_str() : initial_data[i][1].c_str());

            mvwprintw(_launcher->get_sw_content(),
                START_Y + i,
                x_third_col + (col_forth_length) / 2 - initial_data[i][2].size() / 2,
                "%s",
                initial_data[i][2].c_str());

            mvwprintw(_launcher->get_sw_content(),
                START_Y + i,
                x_forth_col + col_fifth_length / 2 - initial_data[i][3].size() / 2,
                "%s",
                initial_data[i][3].c_str());

            mvwprintw(_launcher->get_sw_content(),
                START_Y + i,
                x_fifth_col + col_sixth_length / 2 - initial_data[i][4].size() / 2,
                "%s",
                initial_data[i][4].c_str());

            if (std::stoi(initial_data[i][2]) == 0) {
                mvwprintw(_launcher->get_sw_content(),
                    START_Y + i,
                    x_sixth_col + col_seventh_length / 2,
                    "%s", "0");
            } else {
                long percentage = std::stol(initial_data[i][4]) * 100 / std::stol(initial_data[i][2]);
                mvwprintw(_launcher->get_sw_content(),
                    START_Y + i,
                    x_sixth_col + col_seventh_length / 2 - std::to_string(percentage).size() / 2,
                    "%s",
                    std::to_string(percentage).c_str());
            }
            if (m_current_line == i)
                wattroff(_launcher->get_sw_content(), A_BOLD | COLOR_PAIR(1));
        }
    }
}

void SwActive::spawn_new_download(int m_current_char)
{
    index = m_current_char;
    std::vector<std::string> uris = { initial_data[m_current_char][1] };
    aria2::KeyVals options;

    std::string home = "/home/";
    char* user_name = getenv("USER");
    std::string downloads_directory = "/Downloads";
    std::string downloads_path = home + string(user_name) + downloads_directory;

    options.push_back(std::pair<std::string, std::string>("dir", downloads_path));
    threads.emplace_back(downloaderJob, this, uris, options);
}

void SwActive::prevent_downloaded_files(int index)
{
    if (std::stoi(initial_data[index][2]) == 0) {
        spawn_new_download(index);
    } else {
        long percentage = std::stol(initial_data[index][4]) * 100 / std::stol(initial_data[index][2]);
        if (percentage != 100) {
            spawn_new_download(index);
        }
    }
}

int SwActive::show_active_window()
{
    int c = 0;
    int g = 0;

    wborder(_launcher->get_sw_content(), 0, 0, 0, 0, 0, 0, 0, 0);
    keypad(_launcher->get_sw_content(), TRUE);

    initial_data = _launcher->get_sqlite_util()->get_items();
    PAGE_SIZE = initial_data.size() - 1 < (_launcher->get_y_max() - 8)
        ? initial_data.size()
        : _launcher->get_y_max() - 8; // fixed view of 16 line items per "page"

    wclear(_launcher->get_sw_content());
    _launcher->update_window(_launcher->get_sw_content());
    drawStaticItems();

loop:
    drawDynamicItems();

    c = wgetch(_launcher->get_sw_content());
    switch (c) {
    case KEY_HOME:
        m_current_line = PAGE_TOP;
        m_current_char = FIRST_CHAR;
        goto loop;
    case KEY_END:
        m_current_line = PAGE_SIZE;
        m_current_char = initial_data.size() - 1;
        goto loop;
    case 'k':
    case KEY_UP:
        if (m_current_line > PAGE_TOP)
            m_current_line--;
        else
            m_current_line = PAGE_TOP;

        if (m_current_char > FIRST_CHAR)
            m_current_char--;
        else
            m_current_char = FIRST_CHAR;
        goto loop;
    case 'j':
    case KEY_DOWN:
        if (m_current_line < PAGE_SIZE)
            m_current_line++;
        else
            m_current_line = PAGE_SIZE;
        if (m_current_char < initial_data.size() - 1)
            m_current_char++;
        else
            m_current_char = initial_data.size() - 1;
        goto loop;
    case 'g':
        if (!g) {
            mvwprintw(_launcher->get_sw_footer(), 1, _launcher->get_x_max() - 4, "g");
            wborder(_launcher->get_sw_footer(), 0, 0, 0, 0, 0, 0, 0, 0);
            _launcher->update_window(_launcher->get_sw_footer());
            g++;
            goto loop;
        } else {
            wclear(_launcher->get_sw_footer());
            wborder(_launcher->get_sw_footer(), 0, 0, 0, 0, 0, 0, 0, 0);
            _launcher->update_window(_launcher->get_sw_footer());
            m_current_line = PAGE_TOP;
            m_current_char = FIRST_CHAR;
            g = 0;
            goto loop;
        }
    case 'G':
        m_current_line = PAGE_SIZE;
        m_current_char = initial_data.size() - 1;
        goto loop;
    case 'i':
        show_popup(initial_data[m_current_char][0]);
        goto loop;
    case 's':
        prevent_downloaded_files(m_current_char);
        goto loop;
    case 'n':
        c = 'n';
        break;
    case KEY_F(1):
        c = KEY_F(1);
        break;
    case 'q':
        _launcher->get_sqlite_util()->update_items(initial_data);
        endwin();
        exit(0);
        break;
    default:
        goto loop;
    }
    _launcher->update_window(_launcher->get_sw_content());
    return c;
}
