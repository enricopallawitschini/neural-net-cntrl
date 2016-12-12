#ifndef C_NEURON_H
#define C_NEURON_H

#include <map>
#include <vector>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <cairomm/context.h>
#include <gtkmm/drawingarea.h>

#include "Macros.h"


class c_Neuron : Gtk::Widget {
    public:

    enum NeuronType {
        Input,
        Middle,
        Output
    };
    double x;
    double y;
    double radius;
    int id;
    int layer;
    double value;
    double bias;
    double threshold;
    double fadetime;
    NeuronType type;
    bool selected;
    int fired;

     c_Neuron();
     c_Neuron(std::string cmd);
     ~c_Neuron();
     
    void draw(const Cairo::RefPtr<Cairo::Context>& cr, int layer_w, int layer_y, int neuroncount, int neuronnumber);
    bool is_Me(double x_test, double y_test, bool dont_select);
    void fire();
};

#endif
