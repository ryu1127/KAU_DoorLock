/*
 *  HTTP over TLS (HTTPS) example sketch
 *
 *  This example demonstrates how to use
 *  WiFiClientSecure class to access HTTPS API.
 *  We fetch and display the status of
 *  esp8266/Arduino project continuous integration
 *  build.
 *
 *  Limitations:
 *    only RSA certificates
 *    no support of Perfect Forward Secrecy (PFS)
 *    TLSv1.2 is supported since version 2.4.0-rc1
 *
 *  Created by Ivan Grokhotkov, 2015.
 *  This example is in public domain.
 *  
 *  HTTPS 예제 수정 및 키패드 입력 후 matching 
 */
 
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <Keypad.h>
#include <cstring>
#include <cstdlib>

// 키패드 설정

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {D8, D7, D6, D5}; 
byte colPins[COLS] = {D4, D3, D0};

char bufferKey[4];
char answer[4];

int answer_index = 0;
int key_size = 0; //숫자의 크기
bool input_flag = false; //*을 눌렀을 경우

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

const char* ssid = "*";
const char* password = "*";

const char* host = "서버명(HTTP빼고)";

const int httpsPort = 443;

String line;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char* fingerprint = "SHA 해쉬값";

void setup() {
  Serial.begin(115200);
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match"); //해쉬값 틀릴경우
  }

  String url = "/";

  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  
Serial.println("4개의 키를 입력 후, *을 눌러주세요");

char temp;
while(client.available()){
    temp = client.read();
    answer[answer_index] = temp;
    Serial.print(answer[answer_index]);
    answer_index++;
}
Serial.println();

}

void loop() {
  char inputkey = keypad.getKey();

  if(key_size >= 5){
    Serial.println("초과한 키를 입력하여 자동으로 Flush 됩니다.");
    memset(bufferKey,-1, 4);
    key_size = 0;
  }
  
  else if(inputkey != NO_KEY){
    if(inputkey == '#'){
      memset(bufferKey,-1,4);
      key_size = 0;
      Serial.println("초기화입니다");
    }
    
    else if(inputkey == '*'){
      if(input_flag){ //이미 처음에 눌렀을 경우 비밀번호 확인
        bool true_false = false;
        for(int i = 0; i < 4; i++){
          if(answer[i] != bufferKey[i]){
            true_false = true;
            break;
          }
        }
        if(true_false){
          memset(bufferKey,-1,4);
          Serial.println("틀렸습니다");
        }
        else{
          Serial.println("맞았습니다");
        }
        input_flag = false;
      }
      else { //초기입력
        memset(bufferKey,-1,4);
        input_flag = true;
      }
      key_size = 0;
    }
    
    else{
      bufferKey[key_size] = inputkey;
      Serial.print(bufferKey[key_size]);
      key_size++;
    }
  }
}
