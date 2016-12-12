#define MAX_NEURON_PL 30                    //Neurons per Layer
#define MIN_LAYER_CNT 4
#define LAYER_CNT 4                         //Obsolete when Random Nets are buildt from CNTRL
#define CONNECTION_JUMP 2                   //connections can jump skipping layers (CONNECTION_JUMP - 1)
#define PREDEFINED_THRESHOLD 1.15
#define FADETIME_PER_CLOCK 0.000001         //1000000 clocks per second, that gets us 1s
#define BUFFERSIZE 128
#define CNTRL_SEND_FIFO "/tmp/cntrl_send_fifo"
#define CNTRL_ASK_FIFO "/tmp/cntrl_ask_fifo"
#define CNTRL_CMD "C_CMD"
#define BUILD_CMD "B_CMD"
#define ACK ";"


#ifndef NET_LANGUAGE
#define NET_LANGUAGE

enum NetLanguage : int {
    NET = 0,
    LAYER = 1,
    NEURON = 2,
    CONNECTION = 3,
    CMD_FIRE = 4
};


#endif
