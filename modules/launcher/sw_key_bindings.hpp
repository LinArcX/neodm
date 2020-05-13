#ifndef SW_KEY_BINDINGS_H
#define SW_KEY_BINDINGS_H

#include <launcher.hpp>

class SwKeyBindings {
private:
    std::shared_ptr<Launcher> _launcher;

public:
    SwKeyBindings(std::shared_ptr<Launcher> launcher);
    virtual ~SwKeyBindings();
    int show_window();
};

#endif /* SW_KEY_BINDINGS_H */
