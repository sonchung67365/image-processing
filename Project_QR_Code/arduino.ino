#include <Servo.h>

#define pinServo 9
Servo sg90;
char serialData;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial){;}
  //
  pinMode(LED_BUILTIN, OUTPUT);
  //
  sg90.attach(pinServo);
  sg90.write(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0){
    serialData = Serial.read();
    Serial.print(serialData);
    
    if(serialData == '1'){
      digitalWrite(LED_BUILTIN, 1);
      sg90.write(90);
      Serial.println("ON");
      }
    else if(serialData == '0') {
      digitalWrite(LED_BUILTIN, 0);
      sg90.write(0);
      Serial.println("OFF");
      }
  }
}
