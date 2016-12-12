#include <string.h> 
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <sys/stat.h>
//#include <boost/interprocess/ipc/message_queue.hpp>

#include "Macros.h"

class Connector
{   
    public:
    enum RoleModel{
        server,
        client
    };
    char In_Buffer[BUFFERSIZE];
    char Out_Buffer[BUFFERSIZE];
    bool available;

    Connector(RoleModel r, std::string A_FIFO, std::string S_FIFO);
    ~Connector();
    int update();


    protected:
    bool isConnected();
    int sendFIFO(int pipe, std::string content);
    std::string readFIFO(int pipe);

    bool HL_connected;
    bool connected;
    std::string Ask_FIFO, Send_FIFO;
    int Ask_Pipe, Send_Pipe;
    RoleModel role;
    
};
