char mystr[10]; //Initialized variable to store recieved data

void setup() {
  // Begin the Serial at 9600 Baud
  SerialUSB.begin(9600);
  Serial.begin(9600);
  while(!SerialUSB);
  SerialUSB.println("Init");
  Serial1.begin(9600);
  
}

void loop() {
  Serial.readBytes(mystr,5); //Read the serial data and store in var
  Serial1.readBytes(mystr,5);
  SerialUSB.println(mystr); //Print data on Serial Monitor
  delay(1000);
  
}
