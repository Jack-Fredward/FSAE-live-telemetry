unsigned char incomingByte = 0;
unsigned char in_buff[20];

void setup() {
  Serial.begin(115200);
}

void loop() {
  in_buff[20] = 0; 
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    // incomingByte = Serial.read();
    Serial.readBytesUntil('\n',in_buff,20);
    if ((char)in_buff[0] == 'F') {
      //Serial.println((char)in_buff[1]);
      //Serial.println((char)in_buff[2]);
      //Serial.println((char)in_buff[3]);
      Serial.println();
      char A = (char)in_buff[1];
      char B = (char)in_buff[2];
      char C = (char)in_buff[3];
      int a = (A - '0');
      int b = (B - '0');
      int c = (C - '0');
      int d = (100*a + 10*b + c);
      //Serial.println(a);
      //Serial.println(b);
      //Serial.println(c);
      Serial.println(d);
      Serial.println();
      Serial.println((d/6.89475729)/10);
      Serial.println();

      
    }

//    Serial.print((char)in_buff[1]);
//    Serial.println("break");
//    // (int)in_buff[1]*1000;
//    Serial.print((int)in_buff[1]);
//    Serial.println("break2");
//    Serial.print((char)in_buff[3];

    // say what you got:
    //Serial.print("I received: ");
    //Serial.println((char*)in_buff);
  }
}
