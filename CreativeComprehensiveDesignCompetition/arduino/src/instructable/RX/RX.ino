unsigned long StartTime = millis();
unsigned long Value;
unsigned long EndTime;
int Message;
boolean StartedCounting = false;

void setup(){
  Serial.begin(9600);
}

void loop(){
  int sensorValue = analogRead(A0);
  delay(1);
  // Serial.println(sensorValue);

  if(StartedCounting==false && sensorValue>850) {
    Value = millis() - StartTime;
    Message = Value / 1000;
    StartedCounting = false;
    Serial.print("Message: ");
  }
  
  if(StartedCounting==true && sensorValue>850) {
    StartTime = millis();
    StartedCounting = true;
  }
}
