int Message1 = 6;
int Message2 = 2;
boolean Message1done = false;
int i=0;
int j=0;

void setup() {
  pinMode(10, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if(i < Message1) {
    digitalWrite(10,HIGH);
    i++;
    delay(1010);
  } else if( Message1done == false) {
    digitalWrite(10,LOW);
    delay(1000);
    Message1done = true;
  }

  if((j < Message2) && (Message1done == true)) {
    digitalWrite(10,HIGH);
    j++;
    delay(1010);
  }else if(Message1done == true) {
    digitalWrite(10,Low);
    exit(0);
  }
}
