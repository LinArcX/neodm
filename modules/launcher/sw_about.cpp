#include <memory>
#include <stdlib.h>
#include <time.h>

#include "sw_about.hpp"

SwAbout::SwAbout(std::shared_ptr<Launcher> launcher)
    : _launcher(launcher)
{
}

SwAbout::~SwAbout() {}

int SwAbout::init(std::string char_range)
{
    int x, y;
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    int ranger_value = std::stol(char_range); //looks at the integer conversion of the ASCII range command line arguement
    if (ranger_value > 128 || ranger_value < 10) {
        return 128;
    } else {
        return ranger_value;
    }
}

void SwAbout::write_line(int char_ranger)
{
    struct timespec start, end;
    start.tv_sec = 0;

    int strlength, display, counter, basex, basey;
    strlength = rand() % 80 + 10;
    basex = (rand() % _launcher->get_x_max()) - 10;
    basey = (rand() % _launcher->get_y_max()) - 25;

    for (counter = 0; counter < strlength; counter++) {
        start.tv_nsec = 15000000 + 1500000; //roughly 0.3 seconds between inputs of characters
        wattroff(_launcher->get_sw_content(), COLOR_PAIR(1));
        display = rand() % char_ranger + 33;
        wattron(_launcher->get_sw_content(), A_BOLD | COLOR_PAIR(2));
        mvwaddch(_launcher->get_sw_content(), basey, basex, display);
        wrefresh(_launcher->get_sw_content());
        if (nanosleep(&start, &end) < 0) {
            goto EXIT;
        }

        wattroff(_launcher->get_sw_content(), A_BOLD);
        wattron(_launcher->get_sw_content(), COLOR_PAIR(1));
        mvwaddch(_launcher->get_sw_content(), basey, basex, display);
        basey += 1;
        if (basey > _launcher->get_y_max() - 10) {
            break;
        }
    }

EXIT:
    endwin();
    _launcher->update_window(_launcher->get_sw_content());
}

int SwAbout::show_about_window()
{
    wclear(_launcher->get_sw_content());
    wborder(_launcher->get_sw_content(), 0, 0, 0, 0, 0, 0, 0, 0);

    int range, c = 0;
    range = init("128");

    std::string neo_qoute = "Neo: Why do my eyes hurt?";
    int neo_qoute_length = neo_qoute.length();

    std::string morpheus_qoute = "Morpheus: You've never used them before.";
    int morpheus_qoute_length = morpheus_qoute.length();

    mvwprintw(_launcher->get_sw_content(),
        _launcher->get_y_max() - 6,
        _launcher->get_x_max() / 2 - neo_qoute_length / 2,
        neo_qoute.c_str());

    mvwprintw(_launcher->get_sw_content(),
        _launcher->get_y_max() - 5,
        _launcher->get_x_max() / 2 - morpheus_qoute_length / 2,
        morpheus_qoute.c_str());

    int app_version_length = _launcher->get_app_name().length() + 2;
    mvwprintw(_launcher->get_sw_content(),
        _launcher->get_y_max() - 5,
        _launcher->get_x_max() - app_version_length,
        _launcher->get_app_name().c_str());

    mvwprintw(_launcher->get_sw_content(),
        _launcher->get_y_max() - 5,
        2,
        _launcher->get_github_link().c_str());

loop:
    nodelay(_launcher->get_sw_content(), TRUE);
    c = wgetch(_launcher->get_sw_content());
    switch (c) {
    case 'n':
    case KEY_F(2):
    case KEY_F(4):
        nodelay(_launcher->get_sw_content(), FALSE);
        break;
    case 'q':
        nodelay(_launcher->get_sw_content(), FALSE);
        endwin();
        break;
    default:
        nodelay(_launcher->get_sw_content(), FALSE);
        write_line(range);
        goto loop;
    }
    wattroff(_launcher->get_sw_content(), COLOR_PAIR(1));
    wattroff(_launcher->get_sw_content(), COLOR_PAIR(2));
    wattron(_launcher->get_sw_content(), COLOR_PAIR(2));

    _launcher->update_window(_launcher->get_sw_content());
    return c;
}
