#include "c_Neuron.h"


c_Neuron::c_Neuron(){
}
c_Neuron::c_Neuron(std::string cmd){
    size_t pos1, pos2;
    std::string tok;
    pos2 = cmd.find(":", 0);
    tok = cmd.substr(0, pos2);
    layer = stoi(tok);
    pos1 = pos2 + 1;
    pos2 = cmd.find(":", pos1);
    tok = cmd.substr(pos1, pos2-pos1);
    id = stoi(tok);
    pos1 = pos2 +1;
    pos2 = cmd.find(":", pos1);
    tok = cmd.substr(pos1, pos2 - pos1);
    bias = stod(tok);
    pos1 = pos2 +1;
    pos2 = cmd.find(":", pos1);
    tok = cmd.substr(pos1, pos2 - pos1);
    threshold = stod(tok, NULL);
    pos1 = pos2 +1;
    pos2 = cmd.find(":", pos1);
    tok = cmd.substr(pos1, pos2 - pos1);
    fadetime = stod(tok);
    pos1 = pos2 +1;
    pos2 = cmd.find(":", pos1);
    tok = cmd.substr(pos1, pos2 - pos1);
    if(tok == "0")  type = NeuronType::Input;
    if(tok == "1")  type = NeuronType::Middle;
    if(tok == "2")  type = NeuronType::Output;
    selected = false;
    fired = -1;
}
c_Neuron::~c_Neuron(){
}
     
void c_Neuron::draw(const Cairo::RefPtr<Cairo::Context>& cr, int layer_w, int layer_y, int neuroncount, int neuronnumber){
    char text[BUFFERSIZE];
    double red;
    double x_distance;
    x_distance = (layer_w - 10 )/ double(neuroncount);
    radius =  (x_distance / 2) - 2;
    if(radius > 16) radius = 16;
    red = id / double(neuroncount);
    y = layer_y;
    x = (neuronnumber * x_distance) - (x_distance / 2);
    sprintf(text, "%i",id);
    cr->save();
    cr->set_source_rgb(0.18, 0.3, 0.45);
    if(type == Input) {
        cr->set_source_rgb(0.1, 0.7, 0.55);
    }
    if(type == Output) {
        cr->set_source_rgb(0.22, 0.4, 0.20);
    }
    cr->move_to(x + radius, y);
    cr->arc(x, y, radius, 0.0, 2.0 * M_PI); // full circle
    cr->fill_preserve();
    cr->set_line_width(radius / 6);
    cr->set_source_rgb(0.20, 0.20, 0.22);
    if(selected) cr->set_source_rgb(0.3, 0.9, 0.5);
    if(fired > 0) {
        cr->set_source_rgb(0.3, 0.8, 0.95);
        ++fired;
        if(fired == 6) {
            fired = 0;
        }
    }
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
    cr->move_to(x -5, y + radius + 5);
    layout->show_in_cairo_context(cr);
    cr->restore();

}

bool c_Neuron::is_Me(double x_test, double y_test, bool dont_select) {
    if(dont_select) {
        selected = false;
        return false;
    }
    if((abs(x - x_test) < radius) && (abs(y - y_test) < radius)) {
        selected = true;
    }
    else {
        selected = false;
    }
    return selected;
}

void c_Neuron::fire() {
    fired = 1;
}