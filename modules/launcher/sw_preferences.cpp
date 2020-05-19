#include "sw_preferences.hpp"

SwPreferences::SwPreferences(std::shared_ptr<Launcher> launcher)
    : _launcher(launcher)
{
}

SwPreferences::~SwPreferences() {}

void preferences_add_clicked_handler(CDKBUTTON* button)
{
    exitOKCDKScreenOf(&button->obj);
}

void preferences_cancel_clicked_handler(CDKBUTTON* button)
{
    exitCancelCDKScreenOf(&button->obj);
}

void SwPreferences::clean_up()
{
    /* Clean up */
    destroyCDKButton(btn_add_preferences);
    destroyCDKButton(btn_cancel_preferences);

    destroyCDKLabel(lbl_default_download_dir);
    destroyCDKEntry(entry_default_download_dir);

    destroyCDKLabel(lbl_host);
    destroyCDKEntry(entry_host);

    destroyCDKLabel(lbl_port);
    destroyCDKEntry(entry_port);

    destroyCDKScreen(cdkscr_preferences);
    endCDK();

    cbreak();
    noecho();
    endwin();
}

void SwPreferences::show_window()
{
BEGIN:
    wclear(_launcher->get_sw_content());
    wborder(_launcher->get_sw_content(), 0, 0, 0, 0, 0, 0, 0, 0);

    int traverse_ret = 0;
    tButtonCallback add_clicked = &preferences_add_clicked_handler;
    tButtonCallback cancel_clicked = &preferences_cancel_clicked_handler;

    /* Initialize the Cdk screen */
    cdkscr_preferences = initCDKScreen(_launcher->get_sw_content());

    char* txt_default_download_dir[1];
    txt_default_download_dir[0] = "Default Download Directory:";
    lbl_default_download_dir = newCDKLabel(cdkscr_preferences, 3, 1,
        txt_default_download_dir, 1, FALSE, FALSE);
    entry_default_download_dir = newCDKEntry(cdkscr_preferences, 2, 2, NULL,
        "", A_NORMAL, ' ', vMIXED,
        (_launcher->get_x_max() * 3) / 6 - 2, 0, _launcher->get_x_max() * 2, TRUE, FALSE);

    char* txt_host[1];
    txt_host[0] = "Host:";
    lbl_host = newCDKLabel(cdkscr_preferences, _launcher->get_x_max() * 3 / 6 + 2, 1,
        txt_host, 1, FALSE, FALSE);
    entry_host = newCDKEntry(cdkscr_preferences, _launcher->get_x_max() * 3 / 6 + 2, 2, NULL,
        "", A_NORMAL, ' ', vMIXED,
        _launcher->get_x_max() * 2 / 6 - 10, 0, _launcher->get_x_max() * 2, TRUE, FALSE);

    char* txt_port[1];
    txt_port[0] = "Port:";
    lbl_port = newCDKLabel(cdkscr_preferences, _launcher->get_x_max() * 5 / 6 - 6, 1,
        txt_port, 1, FALSE, FALSE);
    entry_port = newCDKEntry(cdkscr_preferences, _launcher->get_x_max() * 5 / 6 - 6, 2, NULL,
        "", A_NORMAL, ' ', vMIXED,
        _launcher->get_x_max() * 1 / 6 + 3, 0, _launcher->get_x_max() * 2, TRUE, FALSE);

    btn_add_preferences = newCDKButton(cdkscr_preferences, 2, 5, "Add",
        add_clicked, TRUE, FALSE);

    btn_cancel_preferences = newCDKButton(cdkscr_preferences, 10, 5,
        "Cancel", cancel_clicked, TRUE, FALSE);

    /* Draw widgets */
    drawCDKLabel(lbl_default_download_dir, TRUE);
    drawCDKEntry(entry_default_download_dir, TRUE);

    drawCDKLabel(lbl_host, TRUE);
    drawCDKEntry(entry_host, TRUE);

    drawCDKLabel(lbl_port, TRUE);
    drawCDKEntry(entry_port, TRUE);

    drawCDKButton(btn_add_preferences, TRUE);
    drawCDKButton(btn_cancel_preferences, TRUE);

    /* Allow user to traverse the screen */
    refreshCDKScreen(cdkscr_preferences);
    traverse_ret = traverseCDKScreen(cdkscr_preferences);

    std::string str_entry(getCDKEntryValue(entry_default_download_dir));

    /*User hit 'OK' button */
    if (traverse_ret == 1) {
        if (!str_entry.empty()) {
            //_launcher->get_sqlite_util()->insert_link(getCDKEntryValue(entry_link));
            clean_up();
        } else {
            goto BEGIN;
        }
    }

    _launcher->update_window(_launcher->get_sw_content());
}

//mvwprintw(_launcher->get_sw_content(), 1, 2, "Screens");
//mvwprintw(_launcher->get_sw_content(), 2, 2, "********************");
//mvwprintw(_launcher->get_sw_content(), 3, 2, "F1. About");
//mvwprintw(_launcher->get_sw_content(), 4, 2, "F2. Normal Downloads");
//mvwprintw(_launcher->get_sw_content(), 5, 2, "F3. Torrent Downloads(Comming soon)");
//mvwprintw(_launcher->get_sw_content(), 6, 2, "F4. Key Bindings");
//mvwprintw(_launcher->get_sw_content(), 7, 2, "F5. Preferences");

//chooser:
//    c = wgetch(_launcher->get_sw_content());
//loop:
//    switch (c) {
//    case KEY_F(1):
//    case KEY_F(2):
//    case KEY_F(4):
//        nodelay(_launcher->get_sw_content(), FALSE);
//        break;
//    case 'q':
//        nodelay(_launcher->get_sw_content(), FALSE);
//        endwin();
//        break;
//    default:
//        goto chooser;
//    }
