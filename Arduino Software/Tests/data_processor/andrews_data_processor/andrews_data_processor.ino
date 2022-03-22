unsigned char incomingByte = 0;

// changed something



int FPSecondByte = 0;
int FPFirstByte = 0;

struct packetDataStruct {
  int throttlePositionFirstByte;
  int throttlePositionSecondByte;
  int engineSpeedFirstByte;
  int engineSpeedSecondByte;
  int fuelPressureFirstByte;
  int fuelPressureSecondByte;
  int engineOilPressureFirstByte;
  int engineOilPressureSecondByte;
  int engineOilTemperatureByte;
  int coolantTemperatureByte;
  int gearByte;
  int exhaustLambdaByte;
  int frontBrakePressureFirstByte;
  int frontBrakePressureSecondByte;
  int rearBrakePressureFirstByte;
  int rearBrakePressureSecondByte;
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
  int result = 0;
  for(int i=lastPos; i>0; i--){
    result += ((char)toConvert[i]- '0')*power(10,(lastPos-i));
  }
  return result;
}

// function to convert kPa to psi (6.894 kPa = 1 psi)
// Apply to sensors: Fuel Pressure, Oil Pressure, Front & Rear Brake pressures
float convert_kPa_psi(long rawValue, float divisorConst){
  return ((rawValue/6.89475729)/divisorConst);
}

// Apply to sensors: Engine Speed
float convert_Hz_rpm(long rawValue, int divisorConst){
  return (rawValue/60/divisorConst);
}

// Apply to sensors: Engine Coolant Temp, Engine Oil Temp
float convert_C_F(long rawValue, float divisorConst){
//  Serial.print("Raw Value: ");
//  Serial.println(rawValue);
  int rawValue_d = rawValue/divisorConst;
//  Serial.print("rawValue / divisor Const: ");
//  Serial.println(rawValue_d);
  int converted_sensor_C = (rawValue_d*10)-400;
//  Serial.print("converted sensor C: ");
//  Serial.println(converted_sensor_C);
  int converted_sensor_F = converted_sensor_C*0.1*(1.8)+32;
//  Serial.print("converted sensor F: ");
//  Serial.println(converted_sensor_F);
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
    unsigned char in_buff[5];
    for(int j = 0; j<5; j++){
      in_buff[j]= ' ';
    }
    // read the incoming byte:
    // incomingByte = Serial.read();
    Serial.readBytesUntil('\n',in_buff,5);
    Serial.println((char*)in_buff);
    if ((char)in_buff[0] == 't'){
      packetData.throttlePositionFirstByte = convertHexToDec(in_buff);
    }
    if ((char)in_buff[0] == 'T'){
      packetData.throttlePositionSecondByte = convertHexToDec(in_buff);
    }
    if ((char)in_buff[0] == 'e'){
      packetData.engineSpeedFirstByte = convertHexToDec(in_buff);
    }
    if ((char)in_buff[0] == 'E'){
      packetData.engineSpeedSecondByte = convertHexToDec(in_buff);
    }
    if ((char)in_buff[0] == 'u'){
      packetData.fuelPressureFirstByte = convertHexToDec(in_buff);
    }
    if ((char)in_buff[0] == 'U'){
      packetData.fuelPressureSecondByte = convertHexToDec(in_buff);
    }
    if ((char)in_buff[0] == 'p'){
      packetData.engineOilPressureFirstByte = convertHexToDec(in_buff);
    }
    if ((char)in_buff[0] == 'P'){
      packetData.engineOilPressureSecondByte = convertHexToDec(in_buff);
    }
    if ((char)in_buff[0] == 'O'){
      packetData.engineOilTemperatureByte = convertHexToDec(in_buff);
    }
    if ((char)in_buff[0] == 'W'){
      packetData.coolantTemperatureByte = convertHexToDec(in_buff);
    }
    if ((char)in_buff[0] == 'G'){
      packetData.gearByte = convertHexToDec(in_buff);
    }
    if ((char)in_buff[0] == 'L'){
      packetData.exhaustLambdaByte = convertHexToDec(in_buff);
    }
    if ((char)in_buff[0] == 'F') {
      packetData.frontBrakePressureSecondByte = convertHexToDec(in_buff);
    }
    if ((char)in_buff[0] == 'f') {
      packetData.frontBrakePressureFirstByte = convertHexToDec(in_buff);
    }
    if ((char)in_buff[0] == 'r') {
      packetData.rearBrakePressureFirstByte = convertHexToDec(in_buff);
    }
    if ((char)in_buff[0] == 'R') {
      packetData.rearBrakePressureSecondByte = convertHexToDec(in_buff);
    }

    if((packetData.throttlePositionFirstByte != 0) && (packetData.throttlePositionSecondByte != 0)){
      Serial.print("Throttle Position: ");
      Serial.println(convert_throttle(andrewsConversion(packetData.throttlePositionFirstByte, packetData.throttlePositionSecondByte), 100));
      packetData.throttlePositionFirstByte = 0;
      packetData.throttlePositionSecondByte = 0;
    }

    if((packetData.engineSpeedFirstByte != 0) && (packetData.engineSpeedSecondByte != 0)){
      Serial.print("Engine Speed: ");
      Serial.println(convert_Hz_rpm(andrewsConversion(packetData.engineSpeedFirstByte, packetData.engineSpeedSecondByte), 100));
      packetData.engineSpeedFirstByte = 0;
      packetData.engineSpeedSecondByte = 0;
    }

    if((packetData.fuelPressureFirstByte != 0) && (packetData.fuelPressureSecondByte != 0)){
      Serial.print("Fuel Pressure: ");
      Serial.println(convert_kPa_psi(andrewsConversion(packetData.fuelPressureFirstByte, packetData.fuelPressureSecondByte), 100));
      packetData.fuelPressureFirstByte = 0;
      packetData.fuelPressureSecondByte = 0;
    }

    if((packetData.engineOilPressureFirstByte != 0) && (packetData.engineOilPressureSecondByte != 0)){
      Serial.print("Engine Oil Pressure: ");
      Serial.println(convert_kPa_psi(andrewsConversion(packetData.engineOilPressureFirstByte, packetData.engineOilPressureSecondByte), 100));
      packetData.engineOilPressureFirstByte = 0;
      packetData.engineOilPressureSecondByte = 0;
    }

    if(packetData.engineOilTemperatureByte != 0){
      Serial.print("Engine Oil Temp: ");
      Serial.println(convert_C_F(andrewsConversion(0, packetData.engineOilTemperatureByte), 100));
      packetData.engineOilTemperatureByte = 0;
    }

    if(packetData.coolantTemperatureByte != 0){
      Serial.print("Coolant Temp: ");
      Serial.println(convert_C_F(andrewsConversion(0, packetData.coolantTemperatureByte), 100));
      packetData.coolantTemperatureByte = 0;
    }

    if(packetData.gearByte != 0){
      Serial.print("Gear: ");
      Serial.println(convert_gear(andrewsConversion(0, packetData.gearByte)));
      packetData.gearByte = 0;
    }

    if(packetData.exhaustLambdaByte != 0){
      Serial.print("Exhaust Lambda: ");
      Serial.println(convert_lambda(andrewsConversion(0, packetData.exhaustLambdaByte), 100));
      packetData.exhaustLambdaByte = 0;
    }

    if((packetData.frontBrakePressureFirstByte != 0) && (packetData.frontBrakePressureSecondByte != 0)){
      Serial.print("Front Brake Pressure: ");
      Serial.println(convert_kPa_psi(andrewsConversion(packetData.frontBrakePressureFirstByte, packetData.frontBrakePressureSecondByte), 100));
      packetData.frontBrakePressureFirstByte = 0;
      packetData.frontBrakePressureSecondByte = 0;
    }
    if((packetData.rearBrakePressureFirstByte != 0) && (packetData.rearBrakePressureSecondByte != 0)){
      Serial.print("Rear Brake Pressure: ");
      Serial.println(convert_kPa_psi(andrewsConversion(packetData.rearBrakePressureFirstByte, packetData.rearBrakePressureSecondByte), 100));
      packetData.rearBrakePressureFirstByte = 0;
      packetData.rearBrakePressureSecondByte = 0;
    }
    Serial.println();
  }
}
