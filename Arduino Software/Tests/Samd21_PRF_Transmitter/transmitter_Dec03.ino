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
  unsigned char oilTempData[10];
  unsigned char waterTempData[10];
  unsigned char oilPressureData[10];
  unsigned char lambdaData[10];
  unsigned char gearData[10];
  unsigned char throttlePPosData[10];
  unsigned char brakePressureFData[10];
  unsigned char brakePressureRData[10];
  unsigned char fuelPressureData[10];
  unsigned char engineSpeedData[10];
} myData;

char oilTemp = 'O';
char waterTemp = 'W';
char oilPressure = 'P';
char lambda = 'L';
char gear = 'G';
char throttlePPos = 'T';
char brakePressureF = 'F';
char brakePressureR = 'R';
char fuelPressure = 'U';
char engineSpeed = 'E';

byte tx_buf[sizeof(myData)] = {0};

void setup()
{
  pinMode(LED, OUTPUT);

  SerialUSB.begin(115200);
  Serial1.begin(115200);
  // It may be difficult to read serial messages on startup. The following line
  // will wait for serial to be ready before continuing. Comment out if not needed.
  while (!SerialUSB);
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


void loop()
{
  //  SerialUSB.print("Waiting...");
  unsigned char temp;
  unsigned char tempArr[10];

  for (int i = 0; i < 10; i++) {
    tempArr[i] = ' ';
    myData.sensorData[i] = ' ';
  }


  if (Serial1.available() > 0) {
    Serial1.readBytesUntil(',', tempArr, 10);
    if ((char)tempArr[0] == oilTemp) {
      SerialUSB.print("TempArr = ");
      SerialUSB.print((char*)tempArr);
      SerialUSB.println();
      for (int i = 0; i < 10; i++) {
        myData.oilTempData[i] = tempArr[i];

      }
    }
    else if ((char)tempArr[0] == waterTemp) {
      SerialUSB.print("TempArr = ");
      SerialUSB.print((char*)tempArr);
      SerialUSB.println();
      for (int i = 0; i < 10; i++) {
        myData.waterTempData[i] = tempArr[i];
      }
    }
    else if ((char)tempArr[0] == oilPressure) {
      SerialUSB.print("TempArr = ");
      SerialUSB.print((char*)tempArr);
      SerialUSB.println();
      for (int i = 0; i < 10; i++) {
        myData.oilPressureData[i] = tempArr[i];
      }
    }
    else if ((char)tempArr[0] == lambda) {
      SerialUSB.print("TempArr = ");
      SerialUSB.print((char*)tempArr);
      SerialUSB.println();
      for (int i = 0; i < 10; i++) {
        myData.lambdaData[i] = tempArr[i];

      }
    }
    else if ((char)tempArr[0] == gear) {
      SerialUSB.print("TempArr = ");
      SerialUSB.print((char*)tempArr);
      SerialUSB.println();
      for (int i = 0; i < 10; i++) {
        myData.gearData[i] = tempArr[i];
      }
    }
    else if ((char)tempArr[0] == throttlePPos) {
      SerialUSB.print("TempArr = ");
      SerialUSB.print((char*)tempArr);
      SerialUSB.println();
      for (int i = 0; i < 10; i++) {
        myData.throttlePPosData[i] = tempArr[i];

      }
    }
    else if ((char)tempArr[0] == brakePressureF) {
      SerialUSB.print("TempArr = ");
      SerialUSB.print((char*)tempArr);
      SerialUSB.println();
      for (int i = 0; i < 10; i++) {
        myData.brakePressureFData[i] = tempArr[i];
      }
    }
    else if ((char)tempArr[0] == brakePressureR) {
      SerialUSB.print("TempArr = ");
      SerialUSB.print((char*)tempArr);
      SerialUSB.println();
      for (int i = 0; i < 10; i++) {
        myData.brakePressureRData[i] = tempArr[i];
      }
    }
    else if ((char)tempArr[0] == fuelPressure) {
      SerialUSB.print("TempArr = ");
      SerialUSB.print((char*)tempArr);
      SerialUSB.println();
      for (int i = 0; i < 10; i++) {
        myData.fuelPressureData[i] = tempArr[i];
      }
    }
    else if ((char)tempArr[0] == engineSpeed) {
      SerialUSB.print("TempArr = ");
      SerialUSB.print((char*)tempArr);
      SerialUSB.println();
      for (int i = 0; i < 10; i++) {
        myData.engineSpeedData[i] = tempArr[i];
      }
    }

    memcpy(tx_buf, &myData, sizeof(myData));
    byte zize = sizeof(myData);

    rf95.send((uint8_t *)tx_buf, zize);
    rf95.waitPacketSent();
    SerialUSB.print("Sent Msg");

    byte buf[RH_RF95_MAX_MESSAGE_LEN];
    byte len = sizeof(buf);
    if (rf95.waitAvailableTimeout(2000)) {
      if (rf95.recv(buf, &len)) {
      }
      else {
        SerialUSB.println("Receive failed");
      }
    }
    else {
      SerialUSB.println("No reply, is the receiver running?");
    }
  }
  delay(10);
}
