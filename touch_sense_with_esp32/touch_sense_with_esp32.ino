
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(2,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(touchRead(T0));
  delay(100);

  if(touchRead(T0)<40)
  {
    digitalWrite(2,HIGH);
  }
  else
  {
    digitalWrite(2,LOW);
  }
}
