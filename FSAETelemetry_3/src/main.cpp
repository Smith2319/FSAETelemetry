#include <mbed.h>
#include <errno.h>
#include "main.h"
#include <FATFileSystem.h>
#include <SDBlockDevice.h>


//can id can be 29 bits; 4 bytes
//look up canutil


SDBlockDevice sd(PB_15, PB_14, PB_13, PB_12);
FATFileSystem fs("fs");



int main() {
  CAN can0(PB_9, PB_8,CAN_BAUD); //(TD, RD, BUS freq)
  CANMessage inMsg;
  CANMessage statusMsg;
  unsigned char data[FRAME_SIZE * CAN_FRAMES];// total buffer size is size of data * amount of can frames
  int sDID[SEND_DATA_COUNT] = {1, 2}; //stores ids of frames to be sent wirelessly via xbee. SEND_DATA_COUNT is amount of frames
  unsigned char* sendData = (unsigned char*)malloc(SEND_DATA_COUNT * FRAME_SIZE * sizeof(unsigned char)); // SEND_DATA_COUNT * FRAME_SIZE, set as 20 because of variable lenght array error bs


   sd.init();

   fs.mount(&sd);

  Timer sendTime;
  //sdFile.opendir("Telemetry Data");   //needs error handling for no directory or sd card
  

 //FILE *dataFile = fopen("/sd/Telemetry Data/dataFile.bin", "wb");

//initialize buffer
  for(int id=0; id<CAN_FRAMES; id++){    //goes through each frame, id is id of canmessage
    int frame = id*FRAME_SIZE;                   //position of beginning of each frame; ex data[0], data[10], data[20]...
    data[frame] = id >> 8;               //beginning of each frame, first 8 bytes of message id; Big Endian
    data[frame+1] = id & 255;            //right 8 bits of message id
    for (int j=frame+2; j<frame+10; j++){//sets bytes 3 through 10 of the 10 byte frame to 0
      data[j]= 0x00;
    }
  }

  

  Serial dataSend(PA_2, PA_3); //tx, rx
  event_callback_t event1;
  sendTime.start();
  while(1) {

    if (can0.read(inMsg)){
      for(unsigned int i=inMsg.id * FRAME_SIZE + 2; i <inMsg.id * FRAME_SIZE + 10; i++){
        data[i] = inMsg.data[i];
      }                                   //assigns data from canmessage to bytes 3 through 10 of the 10 byte frame to be sent to the xbee
    }
  if (sendTime.read_ms() >= DATA_SEND_TIME){              //transmits data every 10ms

  for(int i=0; i<SEND_DATA_COUNT; i++){       //nested for loop puts all frames listed in sDID into sendData, which is then sent off via xbee
    for(int loc = 0; loc < FRAME_SIZE; loc++){
      sendData[FRAME_SIZE*i +loc] = data[sDID[i] * FRAME_SIZE + loc];
    }
  }
    dataSend.write((const unsigned char *) sendData, SEND_DATA_COUNT * FRAME_SIZE, event1, SERIAL_EVENT_TX_COMPLETE); //data sent via xbee

  }

if (sendTime.read_ms() >= DATA_WRITE_TIME) {
    
 //   fwrite(data, 1, FRAME_SIZE * CAN_FRAMES, dataFile); //data written to sdcard
    sendTime.reset();
  }

  }
}