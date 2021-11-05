void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Sender Init");
}

void loop(){  
  // put your main code here, to run repeatedly:
  unsigned char buf[3];
  buf[0] = 'p';
  buf[1] = 107;
  buf[2] = 186;
  buf[3] = 13;
  for (int i=0; i<4; i++){
    if ((char)buf[i]=='p' ){
      Serial.print((char)buf[i]);
    } else {
      Serial.print(buf[i], HEX);
    }
  }
  Serial.println();
}
