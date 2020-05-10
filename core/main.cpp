#include <launcher.hpp>
#include <memory>

int main(int argc, char* argv[])
{
    std::shared_ptr<Launcher> _launcher = std::make_shared<Launcher>();

    if (_launcher->init_ncurses()) {
        std::cout << "Your terminal doesn't support ncurses!";
        return -1;
    } else {
        _launcher->draw_sub_windows();
        _launcher->update_window(_launcher->get_screen());
        _launcher->process_input();
    }
    return 0;
}
