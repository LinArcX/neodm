#ifndef SW_NEW_H
#define SW_NEW_H

#include <cdk.h>
#include <cdk/cdkscreen.h>
#include <launcher.hpp>

class SwNew {
private:
    std::shared_ptr<Launcher> _launcher;

    CDKSCREEN* cdkscreen;
    CDKENTRY* entry_link;
    CDKBUTTON* btn_add;
    CDKBUTTON* btn_cancel;

public:
    SwNew(std::shared_ptr<Launcher> launcher);
    virtual ~SwNew();

    void show_new_window(WINDOW* win);
    void clean_up();
};

#endif /* SW_NEW_H */
