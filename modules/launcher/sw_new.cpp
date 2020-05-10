#include <cdk.h>
#include <cdk/cdkscreen.h>

#include <form.h>
#include <memory>

#include "sw_new.hpp"
#include <util.hpp>

SwNew::SwNew(std::shared_ptr<Launcher> launcher)
    : _launcher(launcher)
{
}

SwNew::~SwNew() {}

void add_clicked_handler(CDKBUTTON* button)
{
    exitOKCDKScreenOf(&button->obj);
}

void cancel_clicked_handler(CDKBUTTON* button)
{
    exitCancelCDKScreenOf(&button->obj);
}

void SwNew::clean_up()
{
    /* Clean up */
    destroyCDKButton(btn_add);
    destroyCDKEntry(entry_link);
    destroyCDKScreen(cdkscreen);
    endCDK();

    cbreak();
    noecho();
    endwin();
}

void SwNew::show_new_window(WINDOW* win)
{
BEGIN:
    wclear(win);
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);

    int traverse_ret = 0;
    tButtonCallback add_clicked = &add_clicked_handler;
    tButtonCallback cancel_clicked = &cancel_clicked_handler;

    /* Initialize the Cdk screen */
    cdkscreen = initCDKScreen(win);

    /* Start CDK Colors */
    entry_link = newCDKEntry(cdkscreen, 5, 2, NULL,
        "Username: ", A_NORMAL, ' ', vMIXED,
        _launcher->get_x_max() - 40, 0, _launcher->get_x_max() - 50, TRUE, FALSE);
    btn_add = newCDKButton(cdkscreen, _launcher->get_x_max() - 20, 2, "Add", add_clicked, TRUE, FALSE);
    btn_cancel = newCDKButton(cdkscreen, _launcher->get_x_max() - 10, 2, "Cancel", cancel_clicked, TRUE, FALSE);

    /* Draw the label */
    drawCDKButton(btn_add, TRUE);
    drawCDKButton(btn_cancel, TRUE);
    drawCDKEntry(entry_link, TRUE);

    /* Allow user to traverse the screen */
    refreshCDKScreen(cdkscreen);
    traverse_ret = traverseCDKScreen(cdkscreen);

    std::string str_entry(getCDKEntryValue(entry_link));

    /*User hit 'OK' button */
    if (traverse_ret == 1) {
        if (!str_entry.empty()) {
            _launcher->get_sqlite_util()->insert_link(getCDKEntryValue(entry_link));
            clean_up();
        } else {
            goto BEGIN;
        }
        //log_message(LOG_FILE, getCDKEntryValue(entry_link));
        //log_message(LOG_FILE, "\n");
    }

    _launcher->update_window(win);
}
