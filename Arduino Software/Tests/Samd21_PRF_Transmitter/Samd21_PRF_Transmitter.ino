/*
  Both the TX and RX ProRF boards will need a wire antenna. We recommend a 3" piece of wire.
  This example is a modified version of the example provided by the Radio Head
  Library which can be found here:
  www.github.com/PaulStoffregen/RadioHeadd
*/

#include <SPI.h>

//Radio Head Library:
#include <RH_RF95.h>

// We need to provide the RFM95 module's chip select and interrupt pins to the
// rf95 instance below.On the SparkFun ProRF those pins are 12 and 6 respectively.
RH_RF95 rf95(12, 6);

int LED = 13; //Status LED is on pin 13

int packetCounter = 0; //Counts the number of packets sent
long timeSinceLastPacket = 0; //Tracks the time stamp of last packet received

// The broadcast frequency is set to 921.2, but the SADM21 ProRf operates
// anywhere in the range of 902-928MHz in the Americas.
// Europe operates in the frequencies 863-870, center frequency at 868MHz.
// This works but it is unknown how well the radio configures to this frequency:
//float frequency = 864.1;
float frequency = 921.2; //Broadcast frequency
char mystr[10];

struct dataStruct {
  unsigned char mybuf[10];
} myData;

struct packetDataStruct {
  unsigned char frontBreakPressureFirstByte[5];
  unsigned char frontBreakPressureSecondByte[5];
  unsigned char rearBreakPressureFirstByte[5];
  unsigned char rearBreakPressureSecondByte[5];
} packetData;

byte tx_buf[sizeof(myData)] = {0};

void setup()
{
  pinMode(LED, OUTPUT);

  SerialUSB.begin(115200);
  Serial1.begin(115200);
  // It may be difficult to read serial messages on startup. The following line
  // will wait for serial to be ready before continuing. Comment out if not needed.
  while (!SerialUSB);
  while (!Serial1);
  SerialUSB.println("RFM Client!");

  //Initialize the Radio.
  if (rf95.init() == false) {
    SerialUSB.println("Radio Init Failed - Freezing");
    while (1);
  }
  else {
    //An LED inidicator to let us know radio initialization has completed.
    SerialUSB.println("Transmitter up!");
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
  }

  // Set frequency
  rf95.setFrequency(frequency);

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  // Transmitter power can range from 14-20dbm.
  rf95.setTxPower(14, false);
}
int counter = 0;

void clearArray(unsigned char *array){
  for (int i=0; i<5; i++){
    array[i] = ' ';
  }
}

void insertData(unsigned char *dataStructArray, unsigned char *tempArr){
  for (int i=0; i<5; i++){
    dataStructArray[i]=tempArr[i];
  }
}

void loop()
{
  //  SerialUSB.print("Waiting...");
  unsigned char temp;
  unsigned char tempArr[5];

  for (int i = 0; i < 5; i++){
    tempArr[i] = ' ';
  }
  
  if (Serial1.available() > 0) {
    Serial1.readBytesUntil(',', tempArr, 5);
    //      SerialUSB.print("TempArr = ");
    //      SerialUSB.print((char*)tempArr);
    //      SerialUSB.println();
    //    SerialUSB.print("TempArr = ");
    //    SerialUSB.print((char*)tempArr);
    //    SerialUSB.println();
    if (((char)tempArr[0] == 'f')) {
      SerialUSB.print("TempArr = ");
      SerialUSB.print((char*)tempArr);
      SerialUSB.println();
//      for (int i = 0; i < 5; i++){
//        packetData.frontBreakPressureFirstByte[i] = ' ';    
//      }
      clearArray(packetData.frontBreakPressureFirstByte);
      
//      for (int i = 0; i < 5; i++) {
//        packetData.frontBreakPressureFirstByte[i] = tempArr[i];
//      }
      insertData(packetData.frontBreakPressureFirstByte, tempArr);
      counter++;
    }

    else if (((char)tempArr[0] == 'F')) {
      //      SerialUSB.print("Temp = ");
      //      SerialUSB.print((char)temp);
      SerialUSB.print("TempArr = ");
      SerialUSB.print((char*)tempArr);
      SerialUSB.println();
      for (int i = 0; i < 5; i++){
        packetData.frontBreakPressureSecondByte[i] = ' ';
      }
      for (int i = 0; i < 5; i++) {
        //        myData.mybuf[i]=tempArr[i];
        packetData.frontBreakPressureSecondByte[i] = tempArr[i];
      }
      counter++;
    } else if (((char)tempArr[0] == 'r')) {
      SerialUSB.print("TempArr = ");
      SerialUSB.print((char*)tempArr);
      SerialUSB.println();
      for (int i = 0; i < 5; i++){
        packetData.rearBreakPressureFirstByte[i] = ' ';
      }
      for (int i = 0; i < 5; i++) {
        //        myData.mybuf[i]=tempArr[i];
        packetData.rearBreakPressureFirstByte[i] = tempArr[i];
      }
      counter++;
    } else if (((char)tempArr[0] == 'R')) {
      SerialUSB.print("TempArr = ");
      SerialUSB.print((char*)tempArr);
      SerialUSB.println();
      for (int i = 0; i < 5; i++){
        packetData.rearBreakPressureSecondByte[i] = ' ';
      }
      for (int i = 0; i < 5; i++) {
        //        myData.mybuf[i]=tempArr[i];
        packetData.rearBreakPressureSecondByte[i] = tempArr[i];
      }
      counter++;
    }


    // SENDING MESSAGE LOGIC
//    SerialUSB.print("counter: ");
//    SerialUSB.print(counter);
    if (counter == 4) {
      SerialUSB.println("SENDING MESSAGE");

//      for (int i=0; i < 5; i++){
//        SerialUSB.print((char)packetData.frontBreakPressureFirstByte[i]);
//      }
//      SerialUSB.println();
//      SerialUSB.print((char*)packetData.frontBreakPressureFirstByte);
//      SerialUSB.println();
//      SerialUSB.println();
//      SerialUSB.println((char*)packetData.frontBreakPressureSecondByte);
//      SerialUSB.println((char*)packetData.rearBreakPressureFirstByte);
//      SerialUSB.println((char*)packetData.rearBreakPressureSecondByte);
      memcpy(tx_buf, &packetData, sizeof(packetData));
      byte zize = sizeof(packetData);

      //THIS IS ALSO WHERE I AM WORKING

      rf95.send((uint8_t *)tx_buf, zize);
      rf95.waitPacketSent();
      SerialUSB.println("Sent Msg");
      counter=0;
      //
      //    // Now wait for a reply
      byte buf[RH_RF95_MAX_MESSAGE_LEN];
      byte len = sizeof(buf);
      if (rf95.waitAvailableTimeout(2000)) {
        //      Should be a reply message for us now
        if (rf95.recv(buf, &len)) {
          //        SerialUSB.print("Got reply: ");
          //        SerialUSB.println((char*)buf);
          //        SerialUSB.print(" RSSI: ");
          //        SerialUSB.print(rf95.lastRssi(), DEC);
        }
        else {
          SerialUSB.println("Receive failed");
        }
      }
      else {
        SerialUSB.println("No reply, is the receiver running?");
      }
    } else {
      SerialUSB.println("Did not send message");
    }
  }
  delay(10);
}
