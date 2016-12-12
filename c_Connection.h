#ifndef C_CONNECTION_H
#define C_CONNECTION_H

#include <map>
#include <vector>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <cairomm/context.h>
#include <gtkmm/drawingarea.h>
#include "c_Neuron.h"
#include "Utilities.cpp"

#include "Macros.h"
class c_Connection : public Gtk::Widget {
    public:
    int id;
    int Neuron_id;
    int Partner_id;
    double weight;
    int x1, x2, y1, y2;
    bool selected;

    c_Connection();
    c_Connection(std::string cmd);
    ~c_Connection();

    void draw(const Cairo::RefPtr<Cairo::Context>& cr, std::map<int, c_Neuron*> net_neurons); //, int w, int y, int neuroncount, int neuronnumber);
    bool is_Me(double x_test, double y_test, bool dont_select);

};

#endif