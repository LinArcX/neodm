#ifndef SW_ABOUT_H
#define SW_ABOUT_H

#include <launcher.hpp>

class SwAbout {
private:
    std::shared_ptr<Launcher> _launcher;

public:
    SwAbout(std::shared_ptr<Launcher> launcher);
    virtual ~SwAbout();

    int show_about_window();
    int init(std::string range);
    void write_line(int char_ranger);
};

#endif /* SW_ABOUT_H */
