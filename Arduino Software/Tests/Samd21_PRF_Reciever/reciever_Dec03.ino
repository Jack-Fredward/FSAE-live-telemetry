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

int LED = 13; //Status LED on pin 13

int packetCounter = 0; //Counts the number of packets sent
long timeSinceLastPacket = 0; //Tracks the time stamp of last packet received
// The broadcast frequency is set to 921.2, but the SADM21 ProRf operates
// anywhere in the range of 902-928MHz in the Americas.
// Europe operates in the frequencies 863-870, center frequency at
// 868MHz.This works but it is unknown how well the radio configures to this frequency:
//float frequency = 864.1;
float frequency = 921.2;

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

void setup()
{
  pinMode(LED, OUTPUT);

  SerialUSB.begin(115200);
  // It may be difficult to read serial messages on startup. The following
  // line will wait for serial to be ready before continuing. Comment out if not needed.
  while (!SerialUSB);
  SerialUSB.println("RFM Server!");

  //Initialize the Radio.
  if (rf95.init() == false) {
    SerialUSB.println("Radio Init Failed - Freezing");
    while (1);
  }
  else {
    // An LED indicator to let us know radio initialization has completed.
    SerialUSB.println("Receiver up!");
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
  }

  rf95.setFrequency(frequency);

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  // Transmitter power can range from 14-20dbm.
  rf95.setTxPower(14, false);
}

void loop()
{
  if (rf95.available()) {
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len)) {
      digitalWrite(LED, HIGH); //Turn on status LED
      timeSinceLastPacket = millis(); //Timestamp this packet

      memcpy(&myData, buf, sizeof(myData));
      SerialUSB.println("Got message: ");
      SerialUSB.println("Oil Temperature: " + (char*)myData.oilTempData);
      SerialUSB.println("Water Temperature: " + (char*)myData.waterPressureData);
      SerialUSB.println("Oil Pressure: " + (char*)myData.oilPressureData);
      SerialUSB.println("Lambda: " + (char*)myData.lambdaData);
      SerialUSB.println("Gear: " + (char*)myData.gearData);
      SerialUSB.println("Throttle Petal Position: " + (char*)myData.throttlePPosData);
      SerialUSB.println("Front Break Pressure: " + (char*)myData.brakePressureFData);
      SerialUSB.println("Rear Break Pressure: " + (char*)myData.brakePressureRData);
      SerialUSB.print("Engine Speed: " + (char*)myData.engineSpeedData);

      SerialUSB.println();

      // Send a reply
      uint8_t toSend[] = "Hello Back JP!";
      rf95.send(toSend, sizeof(toSend));
      rf95.waitPacketSent();
      SerialUSB.println("Sent a reply");
      digitalWrite(LED, LOW); //Turn off status LED
    }
    else
      SerialUSB.println("Recieve failed");
  }
  //Turn off status LED if we haven't received a packet after 1s
  if (millis() - timeSinceLastPacket > 1000) {
    digitalWrite(LED, LOW); //Turn off status LED
    timeSinceLastPacket = millis(); //Don't write LED but every 1s
  }
  delay(10);
}
