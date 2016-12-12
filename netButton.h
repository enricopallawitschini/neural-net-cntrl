#ifndef NETBUTTON_H
#define NETBUTTON_H

#include "gtkmm/button.h"
#include <iostream>
#include "MainWindow.h"

//class MainWindow;

class netButton : public Gtk::Button
{
    public:
    netButton(MainWindow * mw, std::string id, int index);

    void on_clicked();
    protected:
    std::string p_Id;
    MainWindow * p_mw;
    int Index;

};

#endif
