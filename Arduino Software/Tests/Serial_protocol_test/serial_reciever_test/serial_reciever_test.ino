void setup() {
  // put your setup code here, t  o run once:
  Serial.begin(115200);
  Serial.println("Reciever Init");
}

void loop() {
  unsigned char buf[8];
  unsigned int len = 0;
  unsigned char test;
  // put your main code here, to run repeatedly:

  test = Serial.read();
  if ((char)test == '\n'){
    Serial.print((char)test);
  } else {
    Serial.print((char)test);
//    Serial.print(test, HEX);
  }
//  Serial.print((char)Serial.read());
//  Serial.readBytesUntil('\n', buf, len);
//  for(int i=0; i<len; i++){
//    Serial.print(buf[i]);
//  }
//  Serial.println();
}
