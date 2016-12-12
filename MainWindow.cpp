#include "MainWindow.h"


MainWindow::MainWindow() {
    ::remove(CNTRL_ASK_FIFO);
    set_title("Net Display");
    set_default_size(600, 600);
    //maximize();
    set_border_width(10);
    connected2nn = false;
    Askpipe = -1;
    Sendpipe = -1;

    boost::interprocess::message_queue::remove("ctrl_ask");

    Glib::signal_timeout().connect(sigc::mem_fun(*this, &MainWindow::on_timeout), 1);

    m_buttonAsk.add_label("Ask");
    m_buttonAsk.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_buttonAsk_clicked));
    m_buttonCreate.add_label("Create random net");
    m_buttonCreate.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_buttonCreate_clicked));
    m_buttonClose.add_label("Close neural net");
    m_buttonClose.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_buttonClose_clicked));
    add(m_vbox);
    m_vbox.pack_start(m_hbox, Gtk::PACK_SHRINK, 10);
    m_hbox.pack_start(m_buttonAsk, Gtk::PACK_SHRINK, 10);
    m_hbox.pack_start(m_buttonCreate, Gtk::PACK_SHRINK, 10);
    m_hbox.pack_start(m_buttonClose, Gtk::PACK_SHRINK, 10);

    m_vbox.pack_start(nd);
    show_all_children();
}

MainWindow::~MainWindow() {
    if(isConnected()){
        try {
            char message[BUFFERSIZE];
            strcpy(message, CNTRL_CMD);
            strcat(message, ":END");
            boost::interprocess::message_queue mq_ask(boost::interprocess::open_only, "ctrl_ask");
            mq_ask.send(&message, sizeof(message), 0);
        }
        catch(boost::interprocess::interprocess_exception &ex){
            printf("Error button_show :%s\n", ex.what());
        }
    }
    ::close(Askpipe);
    ::close(Sendpipe);
    ::remove(CNTRL_ASK_FIFO); /* Delete the created fifo */
    boost::interprocess::message_queue::remove("ctrl_ask");

}

int MainWindow::addButton(std::string name) {
    std::string id = name.substr(8);
    netButton* m_buttonShow = new netButton(this, id, v_but.size());
    m_buttonShow->set_label(name);
    v_but.push_back(m_buttonShow);
    m_hbox.pack_start(* m_buttonShow, Gtk::PACK_SHRINK, 10);

    //m_buttonShow->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_buttonShow_clicked));
    m_buttonShow->show();
}

void MainWindow::on_buttonShow_clicked(std::string id, int index){
    if(isConnected()){
        try {
            char message[BUFFERSIZE];
            strcpy(message, CNTRL_CMD);
            strcat(message, ":RETRIEVENET:");
            strcat(message, id.c_str());
            boost::interprocess::message_queue mq_ask(boost::interprocess::open_only, "ctrl_ask");
            mq_ask.send(&message, sizeof(message), 0);
            //v_but[index]->set_sensitive(false);
        }
        catch(boost::interprocess::interprocess_exception &ex){
            printf("Error button_show :%s\n", ex.what());
        }
    }
}

void MainWindow::on_buttonAsk_clicked() {
    if(isConnected()){
        try {
            char message[BUFFERSIZE];
            strcpy(message, CNTRL_CMD);
            strcat(message, ":GETNETS");
            boost::interprocess::message_queue mq_ask(boost::interprocess::open_only, "ctrl_ask");
            mq_ask.send(&message, sizeof(message), 0);
        }
        catch(boost::interprocess::interprocess_exception &ex){
            printf("Error button_ask :%s\n", ex.what());
        }
        return;
    }
}

void MainWindow::on_buttonCreate_clicked() {
    if(isConnected()){
        try {
            char message[BUFFERSIZE];
            strcpy(message, CNTRL_CMD);
            strcat(message, ":CREATE_RN:6:6:7");
            boost::interprocess::message_queue mq_ask(boost::interprocess::open_only, "ctrl_ask");
            mq_ask.send(&message, sizeof(message), 0);
        }
        catch(boost::interprocess::interprocess_exception &ex){
            printf("Error button_ask :%s\n", ex.what());
        }
        return;
    }
}

void MainWindow::on_buttonClose_clicked() {
    if(isConnected()){
        try {
            char message[BUFFERSIZE];
            strcpy(message, CNTRL_CMD);
            strcat(message, ":CLOSE");
            boost::interprocess::message_queue mq_ask(boost::interprocess::open_only, "ctrl_ask");
            mq_ask.send(&message, sizeof(message), 0);
        }
        catch(boost::interprocess::interprocess_exception &ex){
            printf("Error button_close: %s\n", ex.what());
        }
        connected2nn = false;
        nd.connected2nn = false;
        for(int i = 0; i < v_but.size(); ++i) {
            delete v_but[i];
        }
        v_but.clear();
        //if(cN) delete cN;
    }
}

bool MainWindow::on_timeout() {
    int pipe, err;
    std::string cmd, old_cmd;
    size_t result;

        // Update Buttons
    updateButtons();
    /* force our program to redraw the entire area.*/
    auto win = get_window();
    if (win) {
        Gdk::Rectangle r(0, 0, get_allocation().get_width(), get_allocation().get_height());
        win->invalidate_rect(r, false);
    }
    if(isConnected()) {
        try {
            boost::interprocess::message_queue mq_send(boost::interprocess::open_only, "ctrl_send");
            while(mq_send.get_num_msg() > 0) {
                char message[BUFFERSIZE];
                unsigned int priority;
                boost::interprocess::message_queue::size_type recvd_size;
                mq_send.receive(&message, sizeof(message), recvd_size, priority);
                printf("Client: %s", message);
                cmd = message;  
                if(cmd.size() < 5) return true;

                if(cmd.substr(0, 5) == CNTRL_CMD){
                    cmd = cmd.substr(6);
                    if(cmd.substr(0, 1) == std::to_string(NET)) { //NET
                        char name[BUFFERSIZE];
                        sprintf(name, "Net Id: %s", cmd.substr(2).c_str());
                        addButton(name);
                    }
                    if(cmd.substr(0, 1) == std::to_string(CMD_FIRE)) {
                        int i = stoi(cmd.substr(2));
                        cN->c_neurons[i]->fire();
                    }
                }
                if(cmd.substr(0, 5) == BUILD_CMD){
                    cmd = cmd.substr(6);
                    if(cmd.substr(0, 1) == std::to_string(NET)) { //NET
                        cN = new c_Net(cmd);
                        nd.cN = cN;
                    }
                    if(cmd.substr(0, 1) == std::to_string(LAYER)) { //LAYER
                        cmd = cmd.substr(2);
                        std::string tok;
                        size_t pos = cmd.find(":",0);
                        tok = cmd.substr(0,pos);
                        int NetId = stoi(tok);
                        if(cN->id == NetId){
                            c_Layer * cL = new c_Layer(cmd);
                            cN->c_layers[cL->id] = cL;
                        }
                    }
                    if(cmd.substr(0, 1) == std::to_string(NEURON)) { //Neuron
                        cmd = cmd.substr(2);
                        std::string tok;
                        size_t pos = cmd.find(":",0);
                        tok = cmd.substr(0,pos); //Net_id
                        int NetId = stoi(tok);
                        size_t pos1 = cmd.find(":",pos + 1);
                        tok = cmd.substr(pos + 1,pos1 - pos - 1);
                        int LayerId = stoi(tok);
                        if(cN->id == NetId){
                            cmd = cmd.substr(pos + 1);   // send to c_neuron starting with layer 
                            c_Neuron * cNu = new c_Neuron(cmd);
                            cN->c_neurons[cNu->id] = cNu;
                            cN->c_layers[LayerId]->c_neurons[cNu->id] =cNu;
                        }
                    }
                    if(cmd.substr(0, 1) == std::to_string(CONNECTION)) { //Neuron
                        cmd = cmd.substr(2);
                        std::string tok;
                        size_t pos = cmd.find(":",0);
                        tok = cmd.substr(0,pos); //Net_id
                        int NetId = stoi(tok);
                        if(cN->id == NetId){
                            cmd = cmd.substr(pos + 1);   // send to c_Connection starting with id 
                            c_Connection * cC = new c_Connection(cmd);
                            cN->c_connections[cC->id] = cC;
                            //cN->c_neurons[cNu->id] = cNu;
                            //cN->c_layers[LayerId]->c_neurons[cNu->id] =cNu;
                        }
                    }
                    if(cmd == "ENDNET") {
                        cN->drawable = true;
                    }
                }
            }
        }
        catch(boost::interprocess::interprocess_exception &ex){
            printf("Error while receiving from server :%s\n", ex.what());
        }
        return true;
    }
    return true;
}

bool MainWindow::isConnected() {
    if(connected2nn) {
        nd.connected2nn = true;
        return true;
    }//new code using mqueue
    try{
        boost::interprocess::message_queue mq_ask(boost::interprocess::open_or_create, "ctrl_ask", 100, BUFFERSIZE);
        connected2nn = true;
    }
    catch(boost::interprocess::interprocess_exception &ex){
        //std::cout << ex.what() << std::endl;
        connected2nn = false;
    }
   if(connected2nn) {
       try{
           boost::interprocess::message_queue mq_send(boost::interprocess::open_only, "ctrl_send");
       }
       catch(boost::interprocess::interprocess_exception &ex){
            connected2nn = false;
       }
    }
    return connected2nn; 
    //                                                   old code using FIFO

    connected2nn= access( CNTRL_ASK_FIFO, F_OK ) != -1 ;//have a look if CNTRL_SEND_FIFO exits
    if(!connected2nn) {
        int pipe = mkfifo (CNTRL_ASK_FIFO, 0666);
        connected2nn = pipe > -1;
    }
    if(connected2nn){
        std::string cmd;
        connected2nn=connected2nn && access( CNTRL_SEND_FIFO, F_OK ) != -1 ;//have a look if CNTRL_SEND_FIFO exits
        if(connected2nn) {
            Askpipe = open(CNTRL_ASK_FIFO, O_WRONLY);
            if( Askpipe < 0 )         printf("Could not open fifo ASK \n");
            int err = sendFIFO(Askpipe, "Hi");
            /*std::thread t = std::thread(sendFIFO, Askpipe, "Hi");
            t.detach();
            */
            Sendpipe = open(CNTRL_SEND_FIFO, O_RDONLY);//|O_NONBLOCK);  //
        }
        nd.connected2nn = connected2nn;
        return connected2nn;
    }
    connected2nn = false;
    nd.connected2nn = false;
    return false;
}

int MainWindow::sendFIFO(int pipe, std::string content) {
    char out[BUFFERSIZE];
    int ret;
    
    ret = sprintf(out,"%s", content.c_str());
    if(pipe <= 0){
        return -1;
    } 
    write(pipe, out, strlen(out)+1);
    return 0;
}

std::string MainWindow::readFIFO(int pipe){
    int fifo;
    char buffer[1024];
    size_t result;

    if(pipe <= 0) printf("Fifo open error SEND\n"); 
    result = read(pipe, buffer, 1024); 
    //printf("reading: %s\n", buffer);
    std::string out(buffer);

    return out;
}

void MainWindow::updateButtons() {
    m_buttonAsk.set_sensitive(connected2nn);
    m_buttonClose.set_sensitive(connected2nn);
    m_buttonCreate.set_sensitive(connected2nn);

}