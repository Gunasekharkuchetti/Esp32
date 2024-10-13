String cmd;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available())
  {
    cmd=Serial.readString();
    
  }
  if(cmd=="on")
    {
      digitalWrite(2,HIGH);
    }
  if(cmd=="off")
  {
    digitalWrite(2,LOW);
  }
  

}
