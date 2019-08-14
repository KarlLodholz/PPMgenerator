#include <gtkmm.h>

int main(int argc, char *argv[]) {
    Gtk::Main kit(argc, argv);
    Gtk::Window w;
    Gtk::Button b1("Here's");

    w.set_default_size(640,480);
    w.set_title("PPM Generator");
    w.set_position(Gtk::WIN_POS_CENTER);

    b1.show();
    w.add(b1);

    Gtk::Main::run(w);
    return 0;
}