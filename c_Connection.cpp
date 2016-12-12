#include "c_Connection.h"

c_Connection::c_Connection() {

}

c_Connection::c_Connection(std::string cmd) {
    size_t pos1, pos2;
    std::string tok;
    pos2 = cmd.find(":", 0);
    tok = cmd.substr(0, pos2);
    Neuron_id = stoi(tok);
    pos1 = pos2 + 1;
    pos2 = cmd.find(":", pos1);
    tok = cmd.substr(pos1, pos2-pos1);
    Partner_id = stoi(tok);
    pos1 = pos2 +1;
    //pos2 = cmd.find(":", pos1);
    tok = cmd.substr(pos1, 7);
    weight = strtold(tok.c_str(), NULL);
    double WW = stod(tok);
    id = Neuron_id * 1000000 + Partner_id;
    selected = false;
}

c_Connection::~c_Connection() {

}

void c_Connection::draw(const Cairo::RefPtr<Cairo::Context>& cr, std::map<int, c_Neuron*> net_neurons) { //, int w, int y, int neuroncount, int neuronnumber) {
    
    c_Neuron * temp_neuron = net_neurons[Neuron_id];
    x1 = temp_neuron->x;
    y1 = temp_neuron->y;
    temp_neuron = net_neurons[Partner_id];
    x2 = temp_neuron->x;
    y2 = temp_neuron->y;
    cr->save();
    //                                  get color from weight

    double c_weight = weight+ 0.5;
    cr->set_source_rgb(c_weight, 0.20, c_weight / 2);
    if(selected) cr->set_source_rgb(0.1, 0.9, 0.3);
    cr->set_line_width(2.5);
    cr->move_to(x1 , y1);
    cr->line_to(x2, y2);
    cr->stroke();
    cr->restore();
    //cr->clip();
}

bool c_Connection::is_Me(double x_test, double y_test, bool dont_select) {
    double Distance;
    if(dont_select) {
        selected = false;
        return false;
    }
    Distance = abs(getDistPointLine(x1, y1, x2, y2, x_test, y_test));

    if(Distance < 5.0) {
        selected = true;
    }
    else {
        selected = false;
    }
    return selected;
}