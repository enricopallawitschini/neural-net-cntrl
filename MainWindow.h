#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gtkmm/window.h"
#include "gtkmm/frame.h"
#include <string.h> 
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <boost/interprocess/ipc/message_queue.hpp>

//#include <thread>         // std::thread
#include "NetDisplay.h"
#include "netButton.h"
#include "c_Net.h"

class MainWindow : public Gtk::Window 
{
    public:
    MainWindow();
    virtual ~MainWindow();
    int addButton(std::string name);
    void on_buttonShow_clicked(std::string id, int index);

    bool connected2nn;

    protected:
    //Signal handlers:
    void on_button_clicked();
    void on_buttonAsk_clicked();
    void on_buttonCreate_clicked();
    void on_buttonClose_clicked();

    // Methods
    bool on_timeout();
    bool isConnected();
    void updateButtons();

    static int sendFIFO(int pipe, std::string content);
    std::string readFIFO(int pipe);
    
    int Sendpipe;
    int Askpipe;
    c_Net * cN;                   //Our ControlNet Object (should be a vector/Map?)

    //Child widgets:
    std::vector<Gtk::Button*> v_but;

    Gtk::VBox m_vbox;
    Gtk::HBox m_hbox;
    Gtk::Button m_buttonAsk;
    Gtk::Button m_buttonClose;
    Gtk::Button m_buttonCreate;    
    Gtk::Frame m_Frame;
    NetDisplay nd;



};


#endif //MAINWINDOW_H