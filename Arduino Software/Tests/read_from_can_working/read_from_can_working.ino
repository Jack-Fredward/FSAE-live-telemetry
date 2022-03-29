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

// display the data that we are gathering
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


//    SERIAL_PORT_MONITOR.println(mylen);
//    SERIAL_PORT_MONITOR.print("Hex ");
    for (int i = 0; i < mylen; i++) { // print the data
        
        SERIAL_PORT_MONITOR.print(mybuf[i], HEX);
        SERIAL_PORT_MONITOR.print(" ");
    }
    SERIAL_PORT_MONITOR.println();
//    SERIAL_PORT_MONITOR.print("Dec ");
    for (int i = 0; i < mylen; i++) { // print the data
        
        SERIAL_PORT_MONITOR.print((char)mybuf[i]);
        SERIAL_PORT_MONITOR.print(" ");
    }
    SERIAL_PORT_MONITOR.println();
}

void send_data(unsigned char mybuf[], int len){
    for (int i=0; i<len;i++){
      Serial.print(mybuf[i]);  
    }
    Serial.println();
//    Serial.println((char)mybuf[0]);
//    Serial.println(mybuf[1]);
//    Serial.println(mybuf[2]);
}

// the main body of the program
// This is responsible for reading from the canbus and sending the data where it needs to be
// this loop is repeated forever
void loop() {
    unsigned char len = 0;
    unsigned char canData[8];
    unsigned char mybuf[9];
//    int mylen = 0;

    if (CAN_MSGAVAIL == CAN.checkReceive()) {         // check if data coming
        CAN.readMsgBuf(&len, canData);    // read data,  len: data length, buf: data buf
//        SERIAL_PORT_MONITOR.print("Length");
//        SERIAL_PORT_MONITOR.print(len);
//        SERIAL_PORT_MONITOR.println();
        unsigned long canId = CAN.getCanId();
        switch(canId) {
          case 1600:
          //Throttle position
          Serial.print('t');
          Serial.print(canData[6]);
          Serial.print('T');
          Serial.print(canData[7]);
          //Engine Speed
          Serial.print('e');
          Serial.print(canData[0]);
          Serial.print('E');
          Serial.print(canData[1]);
          break;
          
          case 1601:
          //Fuel Pressure
          Serial.print('u');
          Serial.print(canData[4]);
          Serial.print('U');
          Serial.print(canData[5]);
          break;
          
          case 1604:
          //Oil Pressure
          Serial.print('p');
          Serial.print(canData[6]);
          Serial.print('P');
          Serial.print(canData[7]);
          break;
          
          case 1609:
          //Oil Temp
          Serial.print('O');
          Serial.print(canData[1]);
          //Water Temp
          Serial.print('W');
          Serial.print(canData[0]);
          break;
          
          case 1613:
          //Gear
          Serial.print('G');
          Serial.print(canData[6]);
          break;
          
          case 1617:
          //Lambda
          Serial.print('L');
          Serial.print(canData[0]);
          break;
          
          case 1621:
          //Front Break Pressure
          Serial.print('f');
          Serial.print(canData[0]);
          Serial.print('F');
          Serial.print(canData[1]);
          //Rear Break Pressure
          Serial.print('r');
          Serial.print(canData[2]);
          Serial.print('R');
          Serial.print(canData[3]);
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
