#include <SimpleTimer.h>
#include <elapsedMillis.h>
#include <SD.h>
#include <Servo.h>

unsigned long INTERVAL  = 10000; //30000
const unsigned long HALF = INTERVAL/2;
const int dsize = 8;
elapsedMicros timer0;
long threshold = 0; //To determine 0s and 1s
bool moveToNextBit = false;
const int precision = 1000;
int sensorPin = A0; //pin for ldr
int sensorValue = 0; //Value read in by sensor
int CS_PIN = 10; //SD card pin
int test_led = 13;

SimpleTimer timer;

File file;

String myString="";

bool timeIn = false;

int timer_id;

int servoPin = 9;

Servo servo;

int angle = 0;

volatile unsigned long c_timer;
volatile unsigned long k_timer;
unsigned long counter;


void setup() {
    Serial.begin(9600);
    initializeSD();
    //createFile(fname);

    //Calibrate 1 and 0 threshhold

    for(int i = 0; i < precision; i++)
    {
      threshold += analogRead(sensorPin);
    }
    threshold = threshold / precision;
    threshold = threshold - 15;
    Serial.print("Threshold: ");
    Serial.println(threshold);

    servo.attach(servoPin);
    servo.write(0);

    pinMode(test_led,OUTPUT);
    /*
    //Get file size
    byte buf[4];
    for(int i = 0; i < 4; i++)
    {
      buf[i] = getValue();
    }
    unsigned long fileSize = buf[0] | (buf[1] << 8) | ((unsigned long )buf[2] << 16) | ((unsigned long)buf[3] << 24);
    Serial.print("File Size: ");
    Serial.println(fileSize);

    byte fileNameSize = getValue();
    Serial.print("File Name Size: ");
    Serial.println(fileNameSize);
    
   
    if(fileNameSize == 255) //Would cause for loop to never end
    {
      exit(1);
    }
    Serial.print("File Name: ");
    char fname[] = "";
    for(byte i = 0; i < fileNameSize; i++)
    {
      fname[i] = getValue();
      Serial.print(fname[i]);
    }
    fname[fileNameSize] = '\0'; //Add null terminator
    //fname = "test.txt";
    Serial.println();
    createFile(fname);
    
    //Get file
    for(unsigned long i = 0; i < fileSize; i++)
    {
      file.write(getValue());
    }
    
    Serial.println("Done!");
    file.close();
    */
    

}
        

void loop() {
  //Serial.write(getValue());
  
  
  char myChar = (char)getValue();
  if(myChar == '+') {
    /*
    timer_id = timer.setTimeout(4000,checkTime);
    timeIn=true;
    while(timeIn) {
      timer.run();
      
      if(isAscii(myChar)) {
        myChar = (char)getValue();
        Serial.println(myChar);
        if(myChar == '-') {
          break;
        }
        myString += myChar;
        
      } else {
        break;
      }
    }
    timer.deleteTimer(timer_id);
    */
    //timer_id = timer.setTimeout(4000,checkTime);
    //timeIn=true;
        
          
           
    while(myString.length() < 7) {
      //timer.run();
      //counter += 1;
    
      
      if(isAscii(myChar)) {
        myChar = (char)getValue();
        Serial.println(myChar);
        if(myChar == '-') {
          break;
        } else if(myChar == '@') {
          Serial.println("비트 인식 오류났습니다.");
          break;
        }
        myString += myChar;
        
      } else {
        Serial.println("아스키값이 아닌 문자가 인식됐습니다.");
        break;
      }
    }
    //counter = 0;


    /*
    while(isAscii(myChar)&&timeIn) {
      timer.run();
      myChar = (char)getValue();
      Serial.println(myChar);
      if(myChar == '-') {
        break;
      }
      myString += myChar;
    }
    */
    //timer.deleteTimer(timer_id);
    
  }

  
  
  if(!myString.equals(""))  //myString 값이 있다면
  {
    Serial.println(myString); //시리얼모니터에 myString값 출력
      
      
      if(myString=="open")  //myString 값이 'on' 이라면
      {
        servo.write(180); //LED ON
      } else {
        servo.write(0); //LED ON
      }

      if(myString=="thisiskeyvalue") {
        digitalWrite(test_led,HIGH);
      } else if(myString=="thisiscompanykey") {
        digitalWrite(test_led,LOW);
      }
    myString="";  //myString 변수값 초기화
  }
  
  /*
  char myChar = (char)getValue();
  //while(myChar != "+") {
    if(isAscii(myChar)) {
      myString +=myChar;
      Serial.write(myChar);
    }
    */
  
  /*
  if(!myString.equals(""))  //myString 값이 있다면
  {
    Serial.println("input value: "+myString); //시리얼모니터에 myString값 출력
      
      
      if(myString=="on")  //myString 값이 'on' 이라면
      {
        digitalWrite(13, HIGH); //LED ON
      } else {
        digitalWrite(13, LOW);  //LED OFF
      }
      
      
    myString="";  //myString 변수값 초기화
  }
  */

}

byte getValue()
{
  unsigned long interval = INTERVAL;
  int d[dsize]; //array to store incoming 16 bit values
  bool startBitReceived = false;
  //k_timer = millis();
  while(!startBitReceived)
      {
        
        sensorValue = analogRead(sensorPin); //Takes 100 microseconds
        //k_timer = millis();
        if(sensorValue < threshold) //If we detected start bit
        {
          //if((millis() - k_timer) > 2000) {   //== test
          //  Serial.println("2초 지났어욤~");
          //  return '@'; // @리턴
          //}          
          timer0 = 0;
          startBitReceived = true;     
        }
      }

      //Just delay here until we get to the middle of the start bit
      while(timer0 < HALF)
      {       
      }
      timer0 = 0;

      //Get bits based on timer0
      for(unsigned long i = 0; i < dsize; i++)
      {
        c_timer = millis();       //== test
        while(!moveToNextBit)
        {
          if((millis() - c_timer) > 1000) {   //== test
            Serial.println("1초 지났어욤~");
            return '@'; // @리턴
          }
          
          if(INTERVAL - timer0 < 10)
          {
            d[i] = analogRead(sensorPin);  
            timer0 = 0;
            moveToNextBit = true;
            interval = interval - 100;
          }
        }  
        moveToNextBit = false;
      }
      

      

      byte recVal = convertToDecimal(d);
      
      //Busy wait on stop bit
      timer0 = 0;
      while(timer0 < INTERVAL)
      {}
      return recVal;
}

byte convertToDecimalTest(int arr[dsize])
{
  byte recv = 0;
  for(int i = 0; i < 8; i++)
  {
    if(arr[dsize] <= threshold)
      recv |= true << i;
    else
      recv |= false << i;
  }
  return recv;
}

byte convertToDecimal(int arr[dsize])
{
  int j = dsize;
  byte n;
  byte t = 1;
  byte rec = 0;
  while(j >= 0)
  {
    if(arr[j] <= threshold)
    {
      n = j;
      while(n != 0)
      {
        t=t*2;
        n--; 
      }
      rec = rec + t;
      t = 1;
    }  
    j--;
  }
  return rec;
}

void initializeSD()
{
  Serial.println("Initializing SD card...");
  pinMode(CS_PIN, OUTPUT);

  if (SD.begin(10))
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
}

int createFile(char filename[])
{
  file = SD.open(filename, FILE_WRITE);

  if (file)
  {
    Serial.println("File created successfully.");
    return 1;
  } else
  {
    Serial.println("Error while creating file.");
    return 0;
  }
}

void checkTime() {
  timeIn=false;
  Serial.println(myString);
  myString="";
  Serial.println("타임 아웃ㅎㅎ");
  timer.deleteTimer(timer_id);
}
