#include "Connector.h"

Connector::Connector(RoleModel r, std::string A_FIFO, std::string S_FIFO) {
    role = r;
    Ask_FIFO = A_FIFO;
    Send_FIFO = S_FIFO;
    HL_connected = false;
    connected = false;
}

bool Connector::isConnected() {
    if(connected) {
        return true;
    }
    if(role == server) {
        connected= access(Ask_FIFO.c_str(), F_OK ) != -1 ;
        /*have a look if Ask_FIFO exits, our input    */
        if(connected){
            connected = connected && access(Send_FIFO.c_str(), F_OK ) != -1 ;
            /*have a look if Send_FIFO exits, our output    */
            if(!connected) {
                int pipe = mkfifo(Send_FIFO.c_str(), 0666);
                connected = pipe > -1;
                if(connected) {
                    Ask_Pipe = open(Ask_FIFO.c_str(), O_RDONLY|O_NONBLOCK);
                }
            }
        }
    } else {
        connected= access(Ask_FIFO.c_str(), F_OK ) != -1 ;
        /*have a look if Ask_FIFO exits, the output    */
        if(!connected) {
            int pipe = mkfifo(Ask_FIFO.c_str(), 0666);
        }
        if(connected) {
            connected = connected && access(Send_FIFO.c_str(), F_OK ) != -1 ;
            /*have a look if Send_FIFO exits, the input     */
        }
    
    }
    return connected;
}

int Connector::update() {
    int err = 0;
    std::string cmd = "";

    if(isConnected()) {
        if(role == server) {
            if(!HL_connected) {
                cmd = readFIFO(Ask_Pipe);
                if(cmd == "Hi") {
                    Send_Pipe = open(Send_FIFO.c_str(), O_WRONLY);
                    err = sendFIFO(Send_Pipe, "Ok");
                    HL_connected = true;
                }
            }
            else {

            }
        }
        else {
            if(!HL_connected) {
                Ask_Pipe = open(Ask_FIFO.c_str(), O_WRONLY);
                err = sendFIFO(Ask_Pipe, "Hi");
                Send_Pipe = open(Send_FIFO.c_str(), O_RDONLY);
                cmd = readFIFO(Send_Pipe);
                if(cmd == "Ok") {
                    HL_connected = true;

                }
            }
            else {

            }



        }
    }
    return err;
}

int Connector::sendFIFO(int pipe, std::string content) {
    char out[BUFFERSIZE];
    int ret;
    
    ret = sprintf(out,"%s", content.c_str());
    if(pipe <= 0){
        return -1;
    } 
    write(pipe, out, strlen(out)+1);
    return 0;
}

std::string Connector::readFIFO(int pipe){
    int fifo;
    char buffer[1024];
    size_t result;

    //if(pipe <= 0) printf("Fifo open error SEND\n"); 
    result = read(pipe, buffer, 1024); 
    //printf("reading: %s\n", buffer);
    std::string out(buffer);

    return out;
}