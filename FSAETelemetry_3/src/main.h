#define PRINT_STATUS // uncomment - print status
// #define PRINT_CAN    // uncomment - print CAN

#define CAN_BAUD 196200
#define SERIAL_BAUD 230400 //set to max of xbee 900; can only transfer 288 bytes per 10ms!!
#define ECU_TIMEOUT_MS 250
#define CAN_TIMEOUT_MS 250
#define FRAME_SIZE 10 //size of each frame
#define CAN_FRAMES 100  // # of frames total
#define ID_BYTES 2 // amount of bytes in can id
#define CAN_ID 0 // between 0 and 255
#define SEND_DATA_COUNT 2
#define DATA_SEND_TIME 10   //time interval (ms) to send data over xbee
#define DATA_WRITE_TIME 10  //time interval (ms) to write data to sd card
