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

struct dataStruct{
  char mybuf0[2];
  char mybuf1[1];
  char mybuf2[2];
  char mybuf3[2];
  char mybuf4[2];
  char mybuf5[2];
  char mybuf6[2];
  char mybuf7[1];
  char mybuf8[1];
  int number;
}myData;

byte tx_buf[sizeof(myData)] = {0};

void setup()
{
  pinMode(LED, OUTPUT);

  SerialUSB.begin(115200);
  Serial1.begin(115200);
  // It may be difficult to read serial messages on startup. The following line
  // will wait for serial to be ready before continuing. Comment out if not needed.
//  while(!SerialUSB);
  while(!Serial1); 
  SerialUSB.println("RFM Client!"); 

  //Initialize the Radio.
  if (rf95.init() == false){
    SerialUSB.println("Radio Init Failed - Freezing");
    while (1);
  }
  else{
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

void disp_set_data(char buf[], char data, int len){
  for (int i=0; i<len; i++){
    buf[i]=data;
    SerialUSB.print(buf[i]);
  }
  SerialUSB.println();
}


void loop() {

  disp_set_data(myData.mybuf0, 'a', 2);
  disp_set_data(myData.mybuf1, 'b', 1);
  disp_set_data(myData.mybuf2, 'c', 2);
  disp_set_data(myData.mybuf3, 'd', 2);
  disp_set_data(myData.mybuf4, 'e', 2);
  disp_set_data(myData.mybuf5, 'f', 2);
  disp_set_data(myData.mybuf6, 'g', 2);
  disp_set_data(myData.mybuf7, 'h', 1);
  disp_set_data(myData.mybuf8, 'i', 1);

  memcpy(tx_buf, &myData, sizeof(myData));
  byte zize=sizeof(myData);
  rf95.send((uint8_t *)tx_buf, zize);
  rf95.waitPacketSent();
  delay(10);
}
