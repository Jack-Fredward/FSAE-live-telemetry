unsigned char incomingByte = 0;


int FPSecondByte = 0;
int FPFirstByte = 0;

void setup() {
  Serial.begin(115200);
//  Serial.print("Starting Dataprocessor");
}

int power(int base, int exp) {
    if (exp == 0)
        return 1;
    else if (exp % 2)
        return base * power(base, exp - 1);
    else {
        int temp = power(base, exp / 2);
        return temp * temp;
    }
}

int convertHexToDec(unsigned char toConvert[20]){
//  Serial.println((char*)toConvert);
  int lastPos=0;
  while(toConvert[lastPos]!=' '){
    lastPos++;
  }
//  Serial.print("lastPos=");
//  Serial.print(lastPos);
//  Serial.println();
  int result = 0;
  for(int i=lastPos; i>0; i--){
//    Serial.print("Converted char before power function: ")
//    Serial.print(((char)toConvert[i]- '0'));
    result += ((char)toConvert[i]- '0')*power(10,(lastPos-i));
  }
//  Serial.print("Int: ");
//  Serial.println(result);
  return result;
}

float getFuelPressure(long rawValue, int divisorConst){
  return ((rawValue/6.89475729)/divisorConst);
}

int andrewsConversion(int firstByte, int secondByte){
  return (((FPFirstByte+16) * 256) + (FPSecondByte+16));
}


void loop() {
  //clear in_buff from last read
   
//  Serial.println("Data processing loop");
 
//  Serial.println((char*)in_buff);
  // send data only when you receive data:
  if (Serial.available() > 0) {
    unsigned char in_buff[20];
    for(int j = 0; j<20; j++){
      in_buff[j]= ' ';
    }
    // read the incoming byte:
    // incomingByte = Serial.read();
    Serial.readBytesUntil('\n',in_buff,20);
//    Serial.println((char*)in_buff);
    if ((char)in_buff[0] == 'F') {
      int rawFuelPressure2 = convertHexToDec(in_buff);
//      Serial.print("Fuel Pressure2: ");
//      Serial.println(getFuelPressure(rawFuelPressure2));
      FPSecondByte=rawFuelPressure2;
    }
    if ((char)in_buff[0] == 'f') {
      int rawFuelPressure1 = convertHexToDec(in_buff);
//      Serial.print("Fuel Pressure1: ");
//      Serial.println(getFuelPressure(rawFuelPressure1));
      FPFirstByte=rawFuelPressure1;
    }
//    } else if ((char)in_buff[0] == 'f'){
//      int rawFuelPressure1 = convertHexToDec(in_buff);
//      Serial.print("Fuel Pressure1: ");
//      Serial.println(getFuelPressure(rawFuelPressure1));
//      FPFirstByte = rawFuelPressure1;
//    }

    if((FPFirstByte != 0) && (FPSecondByte != 0)){
     
////      if (FPFirstByte > 0){
//        Serial.print("Andrew's Fuel Pressure: ");
//        Serial.println(((((FPFirstByte+16)/10) * 256) + ((FPSecondByte+16)/10))*0.0145);

//        Serial.print("JP's Fuel Pressure: ");
//        Serial.println(getFuelPressure((((FPFirstByte+16)) * 256) + ((FPSecondByte+16)),100));
//        Serial.println(getFuelPressure(andrewsConversion(FPFirstByte, FPSecondByte),100));
//        Serial.print("First Byte: ");
//        Serial.println(FPFirstByte);
//        Serial.print("Second Byte: ");
//        Serial.println(FPSecondByte);
//        Serial.print("Break Pressure: (psi)");
        Serial.println(getFuelPressure(andrewsConversion(FPFirstByte, FPSecondByte),100));
//      

//      }
//      else{
//        Serial.print("Combined Fuel Pressure: ");
//        Serial.println(getFuelPressure(FPSecondByte));
//        Serial.print("Andrew's Number (2544): ");
//        Serial.println((FPSecondByte)*0.0143);
//        Serial.print("Andrew's Number (256): ");
//        Serial.println((FPSecondByte)*0.0143);
//      }
    }
      //Serial.println((char)in_buff[1]);
      //Serial.println((char)in_buff[2]);
      //Serial.println((char)in_buff[3]);
//      Serial.println();
//      convertHexToDec(in_buff);
//      char A = (char)in_buff[1];
//      char B = (char)in_buff[2];
//      char C = (char)in_buff[3];
//      int a = (A - '0');
//      int b = (B - '0');
//      int c = (C - '0');
//      int d = (100*a + 10*b + c);

//      Serial.println(convertHexToDec(in_buff));
//      int fuelPressureRaw = convertHexToDec(in_buff);
     
//      Serial.println(getFuelPressure(fuelPressureRaw));
      //Serial.println(a);
      //Serial.println(b);
      //Serial.println(c);
//      Serial.println(d);
//      Serial.println();
//      Serial.println(d/6.89475729)/100);
//      Serial.println();

     


//    Serial.print((char)in_buff[1]);
//    Serial.println("break");
//    // (int)in_buff[1]*1000;
//    Serial.print((int)in_buff[1]);
//    Serial.println("break2");
//    Serial.print((char)in_buff[3];

    // say what you got:
    //Serial.print("I received: ");
    //Serial.println((char*)in_buff);
    Serial.println();
  }
}
