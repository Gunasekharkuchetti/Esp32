#include<ESP32Servo.h>
Servo myservo;
int pin=18;
void setup() {
  // put your setup code here, to run once:
  myservo.attach(18);
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available())
  {
    int angle=Serial.parseInt();
    myservo.write(angle);
  }
  delay(100);
}
