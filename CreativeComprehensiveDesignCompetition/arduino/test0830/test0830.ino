#include <SimpleTimer.h>
#include <elapsedMillis.h>
//#include <SD.h>

#include <SPI.h> 
#include <Ethernet.h> 
 

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
//int CS_PIN = 10; //SD card pin
//int test_led = 13;

SimpleTimer timer;

//File file;

String myString = "";
//String keyString = "";

char key[15];
unsigned char cnt = 0;

// 이더넷 컨트롤러를 위한 MAC 주소를 넣어 주세요 
// 새로운 이더넷 실드는 MAC 주소가 스티커에 인쇄되어 있습니다 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 
// 만약 DNS 사용을 원치않는다면 (또한 스케치 크기도 줄이고) 
// 서버의 이름 대신에 숫자로 된 IP 주소를 사용합니다: 
IPAddress server(192,168,0,5);  // 네이버 사전 IP 주소 (DNS 사용 않함) 
//char server[] = "endic.naver.com";   // 네이버 사전 주소(DNS 사용) 
// DHCP로 할당 받지 못한 경우를 위하여 사용할 정적 IP 주소를 설정 
// 합니다 
IPAddress ip(192, 168, 0, 100); 
//IPAddress ip(192, 168, 1, 69); 
 
// 이더넷 클라이언트 인스턴스(객체)를 선언합니다. 웹 서버에 
// 접속하기 위한 인스턴스이며, 서버 80 포트로 접속을 하게 
// 됩니다: 
EthernetClient client; 

int servo1Pin = 9;
int servo2Pin = 10;


Servo servo1;
Servo servo2;


volatile unsigned long c_timer;

char myChar;

char mode = 0;


void setup() {
    Serial.begin(9600);

    //threshold 초기값 설정
    Serial.println(F("threshold 초기값 설정"));
    for(int i = 0; i < precision; i++)
    {
      threshold += analogRead(sensorPin);
    }
    threshold = threshold / precision;
    threshold = threshold - 15;
    //임의로 추가한 부분
    threshold = 850;
    
    Serial.print(F("Threshold: "));
    Serial.println(threshold);

    
    

    //서보모터 설정
    servo1.attach(servo1Pin);
    servo1.write(90);
    servo2.attach(servo2Pin);
    servo2.write(90);

    

}
        

void loop() {

  

  
  if(mode == 0) {
    myChar = (char)getValue(); 
  }
  //lifi 키 받는 부분
  if(myChar == '+') {
    
    while(isAscii(myChar)) {
      myChar = (char)getValue();
      Serial.println(myChar);
      
      if(!isAscii(myChar)){
        mode = 0;
        myString = "";
        Serial.println(F("아스키값이 아닌 문자가 인식됐습니다."));
        break;
      } else if(myChar == '-') {
        mode = 1;
        break;
      } else if(myChar == '@') {
        mode = 0;
        myString = "";
        Serial.println(F("비트 인식 오류났습니다."));
        break;
      } else {
        myString += myChar;
        
      }

      
    }

    Serial.println(myString);
  }

  //키 받고 나면 ㅅ서버 연결
  if(mode == 1) {
    //myChar = '.'; 
    //서버 설정
    while (!Serial) { 
      ; // 시리얼 포트가 연결되기를 기다립니다. 레오나르도 보드만 필요 
    } 
   
    // 이더넷 연결을 시작합니다: 
    if (Ethernet.begin(mac) == 0) { 
      Serial.println(F("DHCP를 이용하여 이더넷을 구성하는데 실패")); 
      // DHCP 대신에 정적 IP 주소로 연결을 시도합니다: 
      Ethernet.begin(mac, ip); 
    } 
    // 이더넷 실드 초기화를 위하여 1초간 기다립니다: 
    delay(1000); 
    Serial.println(F("연결중...")); 
    
   
    // 만약 연결되었다면, 시리얼 모니터로 알립니다: 
    if (client.connect(server, 3000)) { 
      Serial.println(F("연결되었습니다")); 
      mode = 2;
      // HTTP request를 만듭니다: 
      client.println("GET /model/modelKey/7"); 
      //client.println("Host: endic.naver.com"); 
      client.println("Host: 192.168.1.46:3000"); 
      client.println("Connection: close"); 
      client.println();   
    } 
    else { 
      // 서버에 연결되지 않았다면: 
      Serial.println(F("연결되지 않았습니다")); 
      mode = 0;
      myString = "";
    }
  }
  
      // 서버로부터 받은 데이터들이 있다면, 이를 읽어들여 시리얼 모니터로 
    // 출력합니다: 
    
  // keyValue에서 실행됨
  while(client.available()) { 
    char c = client.read(); 
    //httpString += c;
    //Serial.print(c);
    if(c == ':') {
      cnt++;
    }
    if(cnt >= 10 && c != ':' && c != '}' && c != '\"') {
      key[cnt-10] = c;    //key 배열에 값 저장
      cnt++;
    }
    
    if(cnt >= 16) {
      cnt = 0;
      mode = 3;
    }
  }

  
  //key값 확인
  if(mode == 3) {
      
      Serial.println();
      Serial.println();
      
      Serial.print(F("서버 key 값 : "));
      for(int i = 0; i < 6; i++) { // 2 8
        
        Serial.print(key[i]);
      }
      Serial.println();
      Serial.println();
      mode = 4;
  }

  if(mode == 4) {
    if(checkEqual(key, myString)){
      Serial.println(F("일치합니다."));
      //key = "";
      myString = "";

      
      servo1.write(180);
      servo2.write(0);
      delay(3000);
      
      mode = 0;
    } else {
      Serial.println(F("일치하지 않습니다."));
      //key = "";
      digitalWrite(11, HIGH);
      delay(1000);
      digitalWrite(11, LOW);
      
      myString = "";
      mode = 0;
    }
    
  }
  
  
  //파싱하는 부분

  



  

    
  // 만약 서버가 연결을 끊었다면, 클라이언트도 중단합니다: 
  if (mode >= 2&&!client.connected()) {  //mode 1 ?? 2??
      
  
      
    Serial.println(); 
    Serial.println(F("연결을 끊습니다.")); 
    client.stop(); 
    mode = 0;
  }
  
  
   
  
  
}

byte getValue()
{
  unsigned long interval = INTERVAL;
  int d[dsize]; //array to store incoming 16 bit values
  bool startBitReceived = false;
  
  
  while(!startBitReceived)
      {
        
        sensorValue = analogRead(sensorPin); //Takes 100 microseconds

        //Serial.println(sensorValue);// 값 추가한 부분
        
        if(sensorValue < threshold) //If we detected start bit
        {
                    
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
            Serial.println(F("비트 인식 시간 초과했습니다."));
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

bool checkEqual(char key[], String myString) {
  for(int i = 0; i < 6; i++) {
    if(key[i] != myString[i]) {
      //Serial.println(key[i]);
      //Serial.println(myString[i]);
      return false;
    }
  }
  return true;
}
