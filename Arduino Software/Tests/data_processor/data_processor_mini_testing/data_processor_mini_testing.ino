
int FPSecondByte = 0;
int FPFirstByte = 0;

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
  Serial.println((char*)toConvert);
  int lastPos=0;
  while(toConvert[lastPos]!=' '){
    lastPos++;
  }
  Serial.print("lastPos=");
  Serial.print(lastPos);
  Serial.println();
  int result = 0;
  for(int i=lastPos; i>0; i--){
    result += ((char)toConvert[i]- '0')*power(10,(lastPos-i));
  }
  Serial.print("Int: ");
  Serial.println(result);
  return result;
}

float getFuelPressure(long rawValue){
  return ((rawValue/6.89475729)/100);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned char test[20] = {'1','2','2',' '};
  Serial.println(convertHexToDec(test));

}
