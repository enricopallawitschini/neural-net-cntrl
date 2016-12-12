#ifndef C_NET_H
#define C_NET_H

#include <map>
#include <vector>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <cairomm/context.h>
#include <glibmm/main.h>
#include <gtkmm/drawingarea.h>
#include <cmath>


//#include "NetDisplay.h"
#include "c_Layer.h"
#include "c_Connection.h" 
#include "Macros.h"


class c_Net : Gtk::Widget {
    public:
     int size;
     int neuronindex;
     int id;
     char name[20];
     std::map<int, c_Layer*> c_layers;
     std::map<int, c_Neuron*> c_neurons;
     std::map<int, c_Connection*> c_connections;
     bool drawable;
     
     c_Net();
     c_Net(std::string cmd);
     c_Net(int i, int lc);
     ~c_Net();
     
     void draw(const Cairo::RefPtr<Cairo::Context>& cr, int w, int h);
     //Neuron * get_c_Neuron(int i);

};

#endif
