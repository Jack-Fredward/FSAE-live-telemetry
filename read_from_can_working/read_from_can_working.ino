// demo: CAN-BUS Shield, receive data with check mode
// send data coming to fast, such as less than 10ms, you can use this way
// loovee, 2014-6-13
#include <SPI.h>

#define CAN_2515
// #define CAN_2518FD

// Set SPI CS Pin according to your hardware

#if defined(SEEED_WIO_TERMINAL) && defined(CAN_2518FD)
// For Wio Terminal w/ MCP2518FD RPi Hat：
// Channel 0 SPI_CS Pin: BCM 8
// Channel 1 SPI_CS Pin: BCM 7
// Interupt Pin: BCM25
const int SPI_CS_PIN  = BCM8;
const int CAN_INT_PIN = BCM25;
#else

// For Arduino MCP2515 Hat:
// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;
const int CAN_INT_PIN = 2;
#endif


#ifdef CAN_2518FD
#include "mcp2518fd_can.h"
mcp2518fd CAN(SPI_CS_PIN); // Set CS pin
#endif

#ifdef CAN_2515
#include "mcp2515_can.h"
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin
#endif                           

void setup() {
    SERIAL_PORT_MONITOR.begin(115200);

    while (CAN_OK != CAN.begin(CAN_1000KBPS)) {             // init can bus : baudrate = 500k
        SERIAL_PORT_MONITOR.println("CAN init fail, retry...");
        delay(100);
    }
    SERIAL_PORT_MONITOR.println("CAN init ok!");
}

void display_data(char mybuf[]){
//    int mylen = (int)(sizeof(mybuf)/sizeof(mybuf[0]));
//    int mylen = 8;
    int mylen;

    switch(mybuf[0]){
      case 'S':
        mylen=9;
        break;
      case 'R':
        mylen=3;
        break;
      case 'T':
        mylen=3;
        break;
      case 'O':
        mylen=3;
        break;
      case 'U':
        mylen=4;
        break;
      case 'G':
        mylen = 2;
        break;
    }


    SERIAL_PORT_MONITOR.println(mylen);
    SERIAL_PORT_MONITOR.print("Hex ");
    for (int i = 0; i < mylen; i++) { // print the data
        
        SERIAL_PORT_MONITOR.print(mybuf[i], HEX);
        SERIAL_PORT_MONITOR.print(" ");
    }
    SERIAL_PORT_MONITOR.println();
    SERIAL_PORT_MONITOR.print("Dec ");
    for (int i = 0; i < mylen; i++) { // print the data
        
        SERIAL_PORT_MONITOR.print(mybuf[i]);
        SERIAL_PORT_MONITOR.print(" ");
    }
    SERIAL_PORT_MONITOR.println();
}

void loop() {
    unsigned char len = 0;
    unsigned char buf[8];
    unsigned char mybuf[8];
//    int mylen = 0;

    if (CAN_MSGAVAIL == CAN.checkReceive()) {         // check if data coming
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
//        SERIAL_PORT_MONITOR.print("Length");
//        SERIAL_PORT_MONITOR.print(len);
//        SERIAL_PORT_MONITOR.println();
        unsigned long canId = CAN.getCanId();
        switch(canId) {
          case 1600: 
            mybuf[0]='S';
            mybuf[1]=buf[0];
            mybuf[2]=buf[1];
            mybuf[3]='M';
            mybuf[4]=buf[2];
            mybuf[5]=buf[3];
            mybuf[6]='P';
            mybuf[7]=buf[6];
            mybuf[8]=buf[7];
            display_data(mybuf);
          break;
          case 1601:
            mybuf[0]='R';
            mybuf[1]=buf[4];
            mybuf[2]=buf[5];
            display_data(mybuf);

          break;
          case 1602:
          mybuf[0]='T';
          mybuf[1]=buf[0];
          mybuf[2]=buf[1];
          display_data(mybuf);

          break;
          case 1604:
          mybuf[0]='O';
          mybuf[1]=buf[6];
          mybuf[2]=buf[7];
          display_data(mybuf);

          break;
          case 1609:
          mybuf[0]='U';
          mybuf[1]=buf[0];
          mybuf[2]='V';
          mybuf[3]=buf[5];
          display_data(mybuf);

          break;
          case 1613:
          mybuf[0]='G';
          mybuf[1]=buf[6];
          display_data(mybuf);

          break;
        }

        delay(10);
        
    }
    else {
//      SERIAL_PORT_MONITOR.println(CAN.checkReceive());
    }
    
}

/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
