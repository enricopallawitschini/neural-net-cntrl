#ifndef NETDISPLAY_H
#define NETDISPLAY_H

#include <gtkmm/drawingarea.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <cmath>
#include <string.h> 
#include <unistd.h>
#include <iostream>
#include <vector>
#include <cairomm/context.h>
#include <glibmm/main.h>
#include <fcntl.h>
#include <boost/interprocess/ipc/message_queue.hpp>

#include "Macros.h"
#include "c_Net.h"

class MainWindow;

class NetDisplay : public Gtk::DrawingArea {

public:
    NetDisplay();
    virtual ~NetDisplay();

    bool connected2nn;
    c_Net * cN;
    
    void draw_rectangle(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
    void draw_text(const Cairo::RefPtr<Cairo::Context>& cr, std::string text, int rectangle_width, int rectangle_height);

protected:
    //Signal handlers:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    // Override mouse events
    bool on_button_press_event(GdkEventButton * event);    
    double m_line_width;

    private:

};

#endif // NETDISPLAY_H