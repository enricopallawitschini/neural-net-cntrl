#include "NetDisplay.h"

NetDisplay::NetDisplay()
: m_line_width(0.05) {
    connected2nn = false;
    add_events(Gdk::BUTTON_PRESS_MASK);
}

NetDisplay::~NetDisplay() {

}

bool NetDisplay::on_button_press_event(GdkEventButton *event) {
    bool has_selected = false;
    if(!cN) return true;
    if((event->type == GDK_BUTTON_PRESS) && (event->button == 1)) {        
        std::map<int, c_Neuron*>::iterator it;
        for(it = cN->c_neurons.begin(); it != cN->c_neurons.end(); ++it) {
            if(it->second->is_Me(event->x, event->y, has_selected)) {
                has_selected = true;
            }
        }
        std::map<int, c_Connection*>::iterator Con_it;
        for(Con_it = cN->c_connections.begin(); Con_it != cN->c_connections.end(); ++Con_it) {
            if(Con_it->second->is_Me(event->x, event->y, has_selected)) {
                has_selected = true;
            }
        }
    }
    if((event->type == GDK_BUTTON_PRESS) && (event->button == 2)) {        
        std::map<int, c_Neuron*>::iterator it;
        for(it = cN->c_neurons.begin(); it != cN->c_neurons.end(); ++it) {
            if(it->second->is_Me(event->x, event->y, has_selected)) {
                has_selected = true;
                   if(connected2nn) {
                    try {
                        char message[BUFFERSIZE];
                        strcpy(message, CNTRL_CMD);
                        strcat(message, ":");
                        strcat(message, std::to_string(CMD_FIRE).c_str());
                        strcat(message, ":");
                        strcat(message, std::to_string(it->second->id).c_str());
                        strcat(message, ":1.00000");
                        boost::interprocess::message_queue mq_ask(boost::interprocess::open_only, "ctrl_ask");
                        mq_ask.send(&message, sizeof(message), 0);
                    }
                    catch(boost::interprocess::interprocess_exception &ex){
                        printf("Error send Fire :%s\n", ex.what());
                    }
                }
            }
        }
    }
}


bool NetDisplay::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    const int rectangle_width = width;
    const int rectangle_height = height;

    // scale to unit square and translate (0, 0) to be (0.5, 0.5), i.e.
    // the center of the window
    //cr->scale(width, height);
    //cr->translate(0.5, 0.5);
    cr->set_line_width(m_line_width);

    cr->save();
    cr->set_source_rgba(0.337, 0.612, 0.117, 0.9);   // green
    cr->paint();
    cr->restore();

    if(!connected2nn) {
        cr->set_source_rgb(1.0, 1.0, 1.0);
        draw_rectangle(cr, rectangle_width, rectangle_height);

        cr->set_source_rgba(0.9, 0.4, 0.15, 0.5);   // Orange
        draw_text(cr, "No Connection!", rectangle_width, rectangle_height/2);
    }
    else {
        if(cN){
            if(cN->drawable) {
                cN->draw(cr, width, height);
            }
        }
    }
    return true;
}

void NetDisplay::draw_rectangle(const Cairo::RefPtr<Cairo::Context>& cr,
                            int width, int height)
{
    cr->rectangle(0, 0, width, height);
    cr->fill_preserve();
    cr->set_line_width(5.0);
    cr->set_source_rgb(0.83, 0.1, 0.22);
    cr->stroke();
}

void NetDisplay::draw_text(const Cairo::RefPtr<Cairo::Context>& cr, std::string text, int rectangle_width, int rectangle_height) {
    // http://developer.gnome.org/pangomm/unstable/classPango_1_1FontDescription.html
    Pango::FontDescription font;

    font.set_family("Monospace");
    font.set_weight(Pango::WEIGHT_BOLD);
    font.set_size(48*PANGO_SCALE);

    // http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html
    auto layout = create_pango_layout(text);

    layout->set_font_description(font);

    int text_width;
    int text_height;

    //get the text dimensions (it updates the variables -- by reference)
    layout->get_pixel_size(text_width, text_height);

    // Position the text in the middle
    cr->move_to((rectangle_width-text_width)/2, (rectangle_height-text_height)/2);

    layout->show_in_cairo_context(cr);
}