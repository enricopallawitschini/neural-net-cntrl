#include <gtkmm/application.h>
#include <clocale>
#include "MainWindow.h"




int main(int argc, char** argv){
    auto app = Gtk::Application::create(argc, argv,"NetDisplay.tris");
    setlocale(LC_ALL, "C");
    MainWindow mw;

    int output = app->run(mw);
    return output;
}

