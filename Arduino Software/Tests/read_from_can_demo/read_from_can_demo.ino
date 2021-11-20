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

// this is the initial setup that the arduino needs in order to function
void setup() {
    SERIAL_PORT_MONITOR.begin(115200);
    Serial.begin(115200);
    
    while (CAN_OK != CAN.begin(CAN_1000KBPS)) {             // init can bus : baudrate = 1000k
        SERIAL_PORT_MONITOR.println("CAN init fail, retry...");
        delay(100);
    }
    SERIAL_PORT_MONITOR.println("CAN init ok!");
}


// the main body of the program
// This is responsible for reading from the canbus and sending the data where it needs to be
// this loop is repeated forever
void loop() {
    unsigned char len = 0;
    unsigned char buf[8];
    unsigned char mybuf[8];
//    int mylen = 0;

    if (CAN_MSGAVAIL == CAN.checkReceive()) {         // check if data coming
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
        unsigned long canId = CAN.getCanId();
        switch(canId) {
//          case 1609:
          case 1601:
          //fuel pressure
//          SERIAL_PORT_MONITOR.print(buf[4]);
//          SERIAL_PORT_MONITOR.print(buf[4], HEX);
//          SERIAL_PORT_MONITOR.print(buf[5], HEX);
//          SERIAL_PORT_MONITOR.print(" ");
          SERIAL_PORT_MONITOR.println(buf[4], DEC);
          SERIAL_PORT_MONITOR.println(buf[4]);
          SERIAL_PORT_MONITOR.println((char)buf[4]);
          SERIAL_PORT_MONITOR.println((buf[4]/6.89475729)/10);
          SERIAL_PORT_MONITOR.println(buf[5], DEC);
          SERIAL_PORT_MONITOR.println(buf[5]);
          SERIAL_PORT_MONITOR.println((char)buf[5]);
          SERIAL_PORT_MONITOR.println((buf[5]/6.89475729)/10);
//          SERIAL_PORT_MONITOR.print(buf[5]);
          Serial.print('f');
          Serial.print(buf[4]);
          Serial.print(',');
          Serial.print('F');
          Serial.print(buf[5]);
          Serial.print(',');
          SERIAL_PORT_MONITOR.println();
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
