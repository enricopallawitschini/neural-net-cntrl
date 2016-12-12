#ifndef C_LAYER_H
#define C_LAYER_H

#include <map>
#include <vector>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <cairomm/context.h>
#include <gtkmm/drawingarea.h>

#include "Macros.h"
#include "c_Neuron.h"

class c_Layer : Gtk::Widget {
    public:

    enum LayerType {
        Input,
        Middle,
        Output
    };
    int size;
    double y;
    int id;
    LayerType type;
    std::map<int, c_Neuron*> c_neurons;

     c_Layer();
     c_Layer(std::string cmd);
     ~c_Layer();
     
     void draw(const Cairo::RefPtr<Cairo::Context>& cr, int w, int h, int layercount, int layer_index);

};

#endif
