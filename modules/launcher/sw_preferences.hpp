#ifndef SW_PREFERENCES_H
#define SW_PREFERENCES_H

#include <cdk.h>
#include <cdk/cdkscreen.h>
#include <launcher.hpp>

class SwPreferences {
private:
    std::shared_ptr<Launcher> _launcher;

    CDKSCREEN* cdkscr_preferences;

    CDKLABEL* lbl_default_download_dir;
    CDKENTRY* entry_default_download_dir;

    CDKLABEL* lbl_host;
    CDKENTRY* entry_host;

    CDKLABEL* lbl_port;
    CDKENTRY* entry_port;

    CDKBUTTON* btn_add_preferences;
    CDKBUTTON* btn_cancel_preferences;

public:
    SwPreferences(std::shared_ptr<Launcher> launcher);
    virtual ~SwPreferences();

    void show_window();
    void clean_up();
};

#endif /* SW_PREFERENCES_H */
