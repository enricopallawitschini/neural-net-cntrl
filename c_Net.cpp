#include <stdio.h>
#include <iostream>
#include "c_Net.h"

c_Net::c_Net() {
    // Uns is fad
    drawable = false;
}

c_Net::c_Net(std::string cmd) {
    drawable = false;
    size_t pos1, pos2;
    std::string tok;
    pos1 = cmd.find(":", 0) + 1;
    pos2 = cmd.find(":", pos1);
    tok = cmd.substr(pos1, pos2 - pos1);
    id = stoi(tok);
    pos1 = pos2 +1;
    pos2 = cmd.find(":", pos1);
    tok = cmd.substr(pos1, pos2 - pos1);
    sprintf(name, "%s", tok.c_str());
    tok = cmd.substr(pos2+1);
    size = stoi(tok);
}

c_Net::c_Net(int i, int lc) {
    id = i;
    neuronindex = 0;
    size = lc;
    sprintf(name, "net%i", id);
}

c_Net::~c_Net() {
    std::map<int, c_Layer*>::iterator L_it;
    for(L_it = c_layers.begin(); L_it != c_layers.end(); ++L_it) {
        delete(L_it->second);
    }
    std::map<int, c_Neuron*>::iterator N_it;
    for(N_it = c_neurons.begin(); N_it != c_neurons.end(); ++N_it) {
        delete(N_it->second);
    }
    std::map<int, c_Connection*>::iterator C_it;
    for(C_it = c_connections.begin(); C_it != c_connections.end(); ++C_it) {
        delete(C_it->second);
    }
}

void c_Net::draw(const Cairo::RefPtr<Cairo::Context>& cr, int w, int h) {
    const int rectangle_width = w;
    const int rectangle_height = h;
    char text[BUFFERSIZE];
    sprintf(text, "Netz Id: %i",id);

    cr->set_source_rgb(0.83, 0.7, 0.55);
    cr->rectangle(0, 0, rectangle_width, rectangle_height);
    cr->fill_preserve();
    cr->set_line_width(5.0);
    cr->set_source_rgb(0.83, 0.1, 0.22);
    cr->stroke();

    Pango::FontDescription font;
    font.set_family("Monospace");
    font.set_weight(Pango::WEIGHT_BOLD);
    font.set_size(14*PANGO_SCALE);
    auto layout = create_pango_layout(text);
    layout->set_font_description(font);
    int text_width;
    int text_height;
    layout->get_pixel_size(text_width, text_height);

    // Position the text in the middle
    cr->move_to((rectangle_width-text_width)/2, 15);

    layout->show_in_cairo_context(cr);

    /* Call Layers to draw themselves
    */
    int layer_index = 0;
    std::map<int, c_Layer*>::iterator it;
    for(it = c_layers.begin(); it != c_layers.end(); ++it){
        it->second->draw(cr, w, h, size, layer_index);
        ++layer_index;
    }
    std::map<int, c_Connection*>::iterator Con_it;
    for(Con_it = c_connections.begin(); Con_it != c_connections.end(); ++Con_it){
        Con_it->second->draw(cr, c_neurons);
    }
}




