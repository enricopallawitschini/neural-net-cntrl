#include "c_Layer.h"


c_Layer::c_Layer(){
}
c_Layer::c_Layer(std::string cmd){
    size_t pos1, pos2;
    std::string tok;
    pos1 = cmd.find(":", 0) + 1;
    pos2 = cmd.find(":", pos1);
    tok = cmd.substr(pos1, pos2 - pos1);
    id = stoi(tok);
    pos1 = pos2 +1;
    pos2 = cmd.find(":", pos1);
    tok = cmd.substr(pos1, pos2 - pos1);
    if(tok == "0")  type = LayerType::Input;
    if(tok == "1")  type = LayerType::Middle;
    if(tok == "2")  type = LayerType::Output;
    tok = cmd.substr(pos2+1);
    size = stoi(tok);


}
c_Layer::~c_Layer(){
    std::map<int, c_Neuron*>::iterator N_it;
    for(N_it = c_neurons.begin(); N_it != c_neurons.end(); ++N_it) {
        delete(N_it->second);
    }
}
     
void c_Layer::draw(const Cairo::RefPtr<Cairo::Context>& cr, int w, int h, int layercount, int layer_index){
    int layer_w = w;
    int layer_h = h / layercount;
    char text[BUFFERSIZE];
    double red;
    red = layer_index / (double(layercount) * 2);
    y = layer_h * layer_index;
    sprintf(text, "Layer Index: %i, Id: %i", layer_index, id);
    cr->save();
    cr->set_source_rgb(red, 0.4, 0.55);
    if(type == Input) {
        cr->set_source_rgb(0.7, 0.7, 0.55);
        strcat(text, " Input");
    }
    if(type == Output) {
        cr->set_source_rgb(0.4, 0.6, 0.8);
        strcat(text, " Output");
    }
    cr->rectangle(0, y, layer_w, layer_h);
    cr->fill_preserve();
    cr->set_line_width(5.0);
    cr->set_source_rgb(red, 0.1, 0.22);
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
    cr->move_to(10, y + 10);

    layout->show_in_cairo_context(cr);

        /* Call c_neurons to draw themselves
    */
    std::map<int, c_Neuron*>::iterator it;
    int i = 0;
    for(it = c_neurons.begin(); it != c_neurons.end(); ++it){
        ++i;
        it->second->draw(cr, layer_w, y + layer_h / 2, size, i);
    }
    cr->restore();
}
