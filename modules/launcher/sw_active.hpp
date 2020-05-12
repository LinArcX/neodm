#ifndef SW_ACTIVE_H
#define SW_ACTIVE_H

#include <aria2/aria2.h>
#include <launcher.hpp>
#include <ncurses.h>
#include <string>
#include <thread>

class SwActive {
private:
    time_t start, end;
    std::vector<std::thread> threads;
    std::shared_ptr<Launcher> _launcher;

    int m_current_line = 0; // line number within m_nPageLines of current selection (0 thru (m_nPageSize - 1))
    int m_current_char = 0; // value of currently selected character

    int PAGE_SIZE; // fixed view of 16 line items per "page"
    const int START_Y = 3;
    const int PAGE_TOP = 0; // first line number on the page of line items
    const int FIRST_CHAR = 0;

    int x_first_col;
    int x_second_col;
    int x_third_col;
    int x_forth_col;
    int x_fifth_col;
    int x_sixth_col;
    int x_seventh_col;
    int x_eighth_col;

    int col_third_length;
    int col_forth_length;
    int col_fifth_length;
    int col_sixth_length;
    int col_seventh_length;
    int col_eighth_length;

    void show_popup(std::string val);
    void drawStaticItems();

public:
    SwActive(std::shared_ptr<Launcher> launcher);
    virtual ~SwActive();

    void drawDynamicItems();
    int show_active_window();
    void spawn_new_download(int m_current_char);
    void prevent_downloaded_files(int index);
};

#endif /* SW_ACTIVE_H */
