unsigned char incomingByte = 0;


int FPSecondByte = 0;
int FPFirstByte = 0;

struct packetDataStruct {
int frontBreakPressureFirstByte = 0;
int frontBreakPressureSecondByte = 0;
int rearBreakPressureFirstByte = 0;
int rearBreakPressureSecondByte = 0;
} packetData;

void setup() {
  Serial.begin(115200);
  Serial.print("Starting Dataprocessor");
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

// function to convert kPa to psi (6.894 kPa = 1 psi)
// Apply to sensors: Fuel Pressure, Oil Pressure, Front & Rear Break pressures
float convert_kPa_psi(long rawValue, float divisorConst){
  return ((rawValue/6.89475729)/divisorConst);
}

// Apply to sensors: Engine Speed
float convert_Hz_rpm(long rawValue, int divisorConst){
  return (rawValue/60/divisorConst);
}

// Apply to sensors: Engine Coolant Temp, Engine Oil Temp
float convert_C_F(long rawValue, float divisorConst){
  Serial.print("Raw Value: ");
  Serial.println(rawValue);
  int rawValue_d = rawValue/divisorConst;
  Serial.print("rawValue / divisor Const: ");
  Serial.println(rawValue_d);
  int converted_sensor_C = (rawValue_d*10)-400;
  Serial.print("converted sensor C: ");
  Serial.println(converted_sensor_C);
  int converted_sensor_F = converted_sensor_C*0.1*(1.8)+32;
  Serial.print("converted sensor F: ");
  Serial.println(converted_sensor_F);
  return converted_sensor_F;
}

// Apply to sensors: Gear
int convert_gear(long rawValue){
  return ((rawValue-320)/10);
}

// Apply to sensors: Trottle Position
float convert_throttle(long rawValue, float divisorConst){
  return (rawValue/divisorConst);
}

// Apply to sensors: Exhaust Lambda
float convert_lambda(long rawValue, float divisorConst){
  return (rawValue/divisorConst);
}

int andrewsConversion(int firstByte, int secondByte){
  return (((firstByte+16) * 256) + (secondByte+16));
}




void loop() {
  //clear in_buff from last read
   
//  Serial.println("Data processing loop");
 
//  Serial.println((char*)in_buff);
  // send data only when you receive data:
  if (Serial.available() > 0) {
    unsigned char in_buff[10];
    for(int j = 0; j<10; j++){
      in_buff[j]= ' ';
    }
    // read the incoming byte:
    // incomingByte = Serial.read();
    Serial.readBytesUntil('\n',in_buff,10);
    Serial.println((char*)in_buff);
    if ((char)in_buff[0] == 'F') {
//      Serial.println((char*)in_buff);
      packetData.frontBreakPressureSecondByte = convertHexToDec(in_buff);
//      Serial.println(frontBreakPressureSecondByte);
//      Serial.print("Fuel Pressure2: ");
//      Serial.println(convert_kPa_psi(rawFuelPressure2));
//      FPSecondByte=rawFuelPressure2;
    }
    if ((char)in_buff[0] == 'f') {
//      Serial.println((char*)in_buff);
      packetData.frontBreakPressureFirstByte = convertHexToDec(in_buff);
//      Serial.println(frontBreakPressureFirstByte);
//      Serial.print("Fuel Pressure1: ");
//      Serial.println(convert_kPa_psi(rawFuelPressure1));
//      FPFirstByte=rawFuelPressure1;
    }
//    } else if ((char)in_buff[0] == 'f'){
//      int rawFuelPressure1 = convertHexToDec(in_buff);
//      Serial.print("Fuel Pressure1: ");
//      Serial.println(convert_kPa_psi(rawFuelPressure1));
//      FPFirstByte = rawFuelPressure1;
//    }
    if ((char)in_buff[0] == 'r') {
      packetData.rearBreakPressureFirstByte = convertHexToDec(in_buff);
    }

    if ((char)in_buff[0] == 'R') {
      packetData.rearBreakPressureSecondByte = convertHexToDec(in_buff);
    }

    if((packetData.frontBreakPressureFirstByte != 0) && (packetData.frontBreakPressureSecondByte != 0)){
//      Serial.println(frontBreakPressureFirstByte);
//      Serial.println(frontBreakPressureSecondByte);
//      Serial.println(andrewsConversion(frontBreakPressureFirstByte, frontBreakPressureSecondByte));
//      int andrewConv = andrewsConversion(frontBreakPressureFirstByte, frontBreakPressureSecondByte);
//      Serial.println(convert_kPa_psi(andrewConv, 100);
      Serial.print("Front Break Pressure: ");
      Serial.println(convert_kPa_psi(andrewsConversion(packetData.frontBreakPressureFirstByte, packetData.frontBreakPressureSecondByte), 100));
      packetData.frontBreakPressureFirstByte = 0;
      packetData.frontBreakPressureSecondByte = 0;
    }
    if((packetData.rearBreakPressureFirstByte != 0) && (packetData.rearBreakPressureSecondByte != 0)){
      Serial.print("Rear Break Pressure: ");
      Serial.println(convert_kPa_psi(andrewsConversion(packetData.rearBreakPressureFirstByte, packetData.rearBreakPressureSecondByte), 100));
      packetData.rearBreakPressureFirstByte = 0;
      packetData.rearBreakPressureSecondByte = 0;
    }
    


//    if((FPFirstByte != 0) && (FPSecondByte != 0)){
//     
//////      if (FPFirstByte > 0){
////        Serial.print("Andrew's Fuel Pressure: ");
////        Serial.println(((((FPFirstByte+16)/10) * 256) + ((FPSecondByte+16)/10))*0.0145);
//
//          Serial.print("Front Break Pressure");
//          Serial.println(convert_kPa_psi(andrewsConversion(FPFirstByte, FPSecondByte),100));
//
////        Serial.print("JP's Fuel Pressure: ");
////        Serial.println(convert_kPa_psi((((FPFirstByte+16)) * 256) + ((FPSecondByte+16)),100));
////        Serial.println(convert_kPa_psi(andrewsConversion(FPFirstByte, FPSecondByte),100));
////        Serial.print("First Byte: ");
////        Serial.println(FPFirstByte);
////        Serial.print("Second Byte: ");
////        Serial.println(FPSecondByte);
////        Serial.print("Break Pressure: (psi)");
////
////        Serial.print("First Byte:");
////        Serial.println(FPFirstByte);
////        Serial.print("Second Byte:");
////        Serial.println(FPSecondByte);
////        Serial.print("Just Andrews Conversion: ");
////        Serial.println(andrewsConversion(-16, FPSecondByte));
////        //Serial.print("Engine Speed:");
////        //Serial.println(convert_Hz_rpm(andrewsConversion(FPFirstByte, FPSecondByte),100));
//////         Serial.print("Coolant Temp (F):");
////         Serial.println(convert_C_F(andrewsConversion(-16, FPSecondByte),10));
//        //Serial.print("Raw Value from 
//        //Serial.println(convert_kPa_psi(andrewsConversion(FPFirstByte, FPSecondByte),100));
//        // Serial.print("Car Gear:");
//        // Serial.println(convert_gear(andrewsConversion(-16, FPSecondByte)));
////        Serial.print("Throttle Position:");
////        Serial.println(convert_throttle(andrewsConversion(FPFirstByte, FPSecondByte),100));
////        Serial.print("Exhaust Lambda:");
////        Serial.println(convert_lambda(andrewsConversion(-16, FPSecondByte),1000));
//        
//
//        
//        
////      
//
////      }
////      else{
////        Serial.print("Combined Fuel Pressure: ");
////        Serial.println(convert_kPa_psi(FPSecondByte));
////        Serial.print("Andrew's Number (2544): ");
////        Serial.println((FPSecondByte)*0.0143);
////        Serial.print("Andrew's Number (256): ");
////        Serial.println((FPSecondByte)*0.0143);
////      }
//    }
//      //Serial.println((char)in_buff[1]);
//      //Serial.println((char)in_buff[2]);
//      //Serial.println((char)in_buff[3]);
////      Serial.println();
////      convertHexToDec(in_buff);
////      char A = (char)in_buff[1];
////      char B = (char)in_buff[2];
////      char C = (char)in_buff[3];
////      int a = (A - '0');
////      int b = (B - '0');
////      int c = (C - '0');
////      int d = (100*a + 10*b + c);
//
////      Serial.println(convertHexToDec(in_buff));
////      int fuelPressureRaw = convertHexToDec(in_buff);
//     
////      Serial.println(convert_kPa_psi(fuelPressureRaw));
//      //Serial.println(a);
//      //Serial.println(b);
//      //Serial.println(c);
////      Serial.println(d);
////      Serial.println();
////      Serial.println(d/6.89475729)/100);
////      Serial.println();
//
//     
//
//
////    Serial.print((char)in_buff[1]);
////    Serial.println("break");
////    // (int)in_buff[1]*1000;
////    Serial.print((int)in_buff[1]);
////    Serial.println("break2");
////    Serial.print((char)in_buff[3];
//
//    // say what you got:
//    //Serial.print("I received: ");
//    //Serial.println((char*)in_buff);
    Serial.println();
  }
}
