#include "sw_key_bindings.hpp"

SwKeyBindings::SwKeyBindings(std::shared_ptr<Launcher> launcher)
    : _launcher(launcher)
{
}

SwKeyBindings::~SwKeyBindings() {}

int SwKeyBindings::show_window()
{
    int c = 0;
    wclear(_launcher->get_sw_content());
    wborder(_launcher->get_sw_content(), 0, 0, 0, 0, 0, 0, 0, 0);

    mvwprintw(_launcher->get_sw_content(), 1, 2, "Screens");
    mvwprintw(_launcher->get_sw_content(), 2, 2, "********************");
    mvwprintw(_launcher->get_sw_content(), 3, 2, "F1. About");
    mvwprintw(_launcher->get_sw_content(), 4, 2, "F2. Normal Downloads");
    mvwprintw(_launcher->get_sw_content(), 5, 2, "F3. Torrent Downloads(Comming soon)");
    mvwprintw(_launcher->get_sw_content(), 6, 2, "F4. Key Bindings");
    mvwprintw(_launcher->get_sw_content(), 7, 2, "F5. Preferences");

    mvwprintw(_launcher->get_sw_content(), 9, 2, "Navigation");
    mvwprintw(_launcher->get_sw_content(), 10, 2, "********************");
    mvwprintw(_launcher->get_sw_content(), 11, 2, "k. Up");
    mvwprintw(_launcher->get_sw_content(), 12, 2, "j. Down");
    mvwprintw(_launcher->get_sw_content(), 13, 2, "gg. Top");
    mvwprintw(_launcher->get_sw_content(), 14, 2, "G. Bottom");

    mvwprintw(_launcher->get_sw_content(), 16, 2, "Actions");
    mvwprintw(_launcher->get_sw_content(), 17, 2, "********************");
    mvwprintw(_launcher->get_sw_content(), 18, 2, "n. New Download");
    mvwprintw(_launcher->get_sw_content(), 19, 2, "s. Start Download");
    mvwprintw(_launcher->get_sw_content(), 20, 2, "p. Puase Download");
    mvwprintw(_launcher->get_sw_content(), 21, 2, "i. Information");
    mvwprintw(_launcher->get_sw_content(), 22, 2, "q. Quit");

chooser:
    c = wgetch(_launcher->get_sw_content());
loop:
    switch (c) {
    case KEY_F(1):
    case KEY_F(2):
        nodelay(_launcher->get_sw_content(), FALSE);
        break;
    case 'q':
        nodelay(_launcher->get_sw_content(), FALSE);
        endwin();
        break;
    default:
        goto chooser;
    }

    _launcher->update_window(_launcher->get_sw_content());
    return c;
}
