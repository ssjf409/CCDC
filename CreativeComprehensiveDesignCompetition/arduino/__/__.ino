/* 
  Web client (웹 클라이언트) 
 
 이 스케치는 아두이노 위즈네트 이더넷 실드를 사용하여 웹 사이트 
 네이버 사전(http://endic.naver.com )에 접속합니다. 
 
 회로: 
 * 이더넷 실드는 디지털 10, 11, 12, 13 핀들과 연결됩니다. 또한 SD 
   카드 칩 선택으로 디지털 4번 핀이 사용됩니다. 
 
 2009년 12월 18일 David A. Mellis에 의하여 창작되었으며, 
 2012년 04월 09일 Adrian McEwen 작업을 기반으로 Tom Igoe에 의하여 
                  수정되었습니다 
 2014년 10월 11일 WayFarer에 의하여 수정되었습니다 
  
 http://blog.naver.com/msyang59/220147949576 
  
 */ 
 
#include <SPI.h> 
#include <Ethernet.h> 
 
// 이더넷 컨트롤러를 위한 MAC 주소를 넣어 주세요 
// 새로운 이더넷 실드는 MAC 주소가 스티커에 인쇄되어 있습니다 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 
// 만약 DNS 사용을 원치않는다면 (또한 스케치 크기도 줄이고) 
// 서버의 이름 대신에 숫자로 된 IP 주소를 사용합니다: 
//IPAddress server(192,168,1,46);  // 네이버 사전 IP 주소 (DNS 사용 않함) 
//char server[] = "endic.naver.com";   // 네이버 사전 주소(DNS 사용) 
char server[] = "www.google.com";   // 네이버 사전 주소(DNS 사용) 
// DHCP로 할당 받지 못한 경우를 위하여 사용할 정적 IP 주소를 설정 
// 합니다 
IPAddress ip(192, 168, 1, 100); 
//IPAddress ip(192, 168, 1, 69); 
 
// 이더넷 클라이언트 인스턴스(객체)를 선언합니다. 웹 서버에 
// 접속하기 위한 인스턴스이며, 서버 80 포트로 접속을 하게 
// 됩니다: 
EthernetClient client; 

String httpString;
 
void setup() { 
  // 시리얼 포트를 열고 연결되기를 기다립니다: 
  Serial.begin(9600); 
  while (!Serial) { 
    ; // 시리얼 포트가 연결되기를 기다립니다. 레오나르도 보드만 필요 
  } 
 
  // 이더넷 연결을 시작합니다: 
  if (Ethernet.begin(mac) == 0) { 
    Serial.println("DHCP를 이용하여 이더넷을 구성하는데 실패"); 
    // DHCP 대신에 정적 IP 주소로 연결을 시도합니다: 
    Ethernet.begin(mac, ip); 
  } 
  // 이더넷 실드 초기화를 위하여 1초간 기다립니다: 
  delay(1000); 
  Serial.println("연결중..."); 
 
  // 만약 연결되었다면, 시리얼 모니터로 알립니다: 
  //if (client.connect(server, 3001)) { 
  if (client.connect(server, 80)) { 
    Serial.println("연결되었습니다"); 
    // HTTP request를 만듭니다: 
    //client.println("GET /model/modelKey/7"); 
    client.println("GET HTTP/1.1"); 
    
    //client.println("Host: endic.naver.com"); 
    client.println("Host: wwww.google.com"); 
    client.println("Connection: close"); 
    client.println(); 
  } 
  else { 
    // 서버에 연결되지 않았다면: 
    Serial.println("연결되지 않았습니다"); 
  } 
} 
 
void loop() 
{ 
  // 서버로부터 받은 데이터들이 있다면, 이를 읽어들여 시리얼 모니터로 
  // 출력합니다: 
  while (client.available()) { 
    char c = client.read(); 
    //httpString += c;
    Serial.print(c); 
  }
  
  //파싱하는 부분
  if(!httpString.equals("")) {
    Serial.println(httpString);
    int first = httpString.indexOf("\":\"");
    String keyString = httpString.substring(first+3, first+9);
    Serial.println(keyString); 
    httpString = "";
  }
 /*
  // 만약 서버가 연결을 끊었다면, 클라이언트도 중단합니다: 
  if (!client.connected()) { 
    

    
    Serial.println(); 
    Serial.println("연결을 끊습니다."); 
    client.stop(); 
 
    // 더이상 아무런 처리도 하지 않습니다: 
    while (true); 
  } 
  */


  while (!Serial) { 
    ; // 시리얼 포트가 연결되기를 기다립니다. 레오나르도 보드만 필요 
  } 
 
  // 이더넷 연결을 시작합니다: 
  if (Ethernet.begin(mac) == 0) { 
    Serial.println("DHCP를 이용하여 이더넷을 구성하는데 실패"); 
    // DHCP 대신에 정적 IP 주소로 연결을 시도합니다: 
    Ethernet.begin(mac, ip); 
  } 
  // 이더넷 실드 초기화를 위하여 1초간 기다립니다: 
  delay(1000); 
  Serial.println("연결중..."); 
 
  // 만약 연결되었다면, 시리얼 모니터로 알립니다: 
  if (client.connect(server, 3001)) { 
    Serial.println("연결되었습니다"); 
    // HTTP request를 만듭니다: 
    client.println("GET /model/modelKey/7"); 
    //client.println("Host: endic.naver.com"); 
    client.println("Connection: close"); 
    client.println(); 
  } 
  else { 
    // 서버에 연결되지 않았다면: 
    Serial.println("연결되지 않았습니다"); 
  }  
  // 서버로부터 받은 데이터들이 있다면, 이를 읽어들여 시리얼 모니터로 
  // 출력합니다: 
  while (client.available()) { 
    char c = client.read(); 
    httpString += c;
    //Serial.print(c); 
  }
  
  //파싱하는 부분
  if(!httpString.equals("")) {
    Serial.println(httpString);
    int first = httpString.indexOf("\":\"");
    String keyString = httpString.substring(first+3, first+9);
    Serial.println(keyString); 
    httpString = "";
  }
}

String compareKey(){
  
}
