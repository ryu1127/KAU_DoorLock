/*
<<<<<<< HEAD
 *  HTTPS 예제 수정 및 키패드 입력 후 matching 
 *  Firebase uid 로 데이터 매칭, 비밀번호 매칭했을때만 열리게하기
=======
 *  Firebase <-> wemos d1 r1 board 
 *  password matching -> doorlock open!! :D
>>>>>>> 62f4df33184c2bde44582525acd8e6e99b185a99
 */
 
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <FirebaseArduino.h>

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

String bufferKey;
String answer;
String my_id = ""; //uid 매칭을 위한 나의 id
<<<<<<< HEAD
=======

int R = D2; //릴레이 연결(솔레노이드 전력 낮춤)
>>>>>>> 62f4df33184c2bde44582525acd8e6e99b185a99

int answer_index = 0;
int key_size = 0; //숫자의 크기
bool input_flag = false; //*을 눌렀을 경우

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

#define FIREBASE_HOST "Firebase 주소"
#define FIREBASE_AUTH "비밀번호"

#define WIFI_SSID "*"
#define WIFI_PASSWORD "*"

String line;

<<<<<<< HEAD
// Use web browser to view and copy
// SHA1 fingerprint of the certificate
// const char* fingerprint = "SHA 해쉬값";

void setup() {
  Serial.begin(115200);

=======
void setup() {
  Serial.begin(115200);

  pinMode(R, OUTPUT); //초기 닫힌 상
  digitalWrite(R, HIGH);
  
>>>>>>> 62f4df33184c2bde44582525acd8e6e99b185a99
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

<<<<<<< HEAD
  /* SHA 해쉬값이용
  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match"); //해쉬값 틀릴경우
  }
  */

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  String compare_id = Firebase.getString("uid/uid_name");
  delay(1000);
  if(my_id != compare_id){
    Serial.println("해당 ID가 아닙니다. 종료");
    return;
  }
  else{
    answer = Firebase.getString("uid/password");
    delay(1000);
    Serial.println("4개의 키를 입력 후, *을 눌러주세요");
  }
=======
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  String compare_id = Firebase.getString("uid/uid_name");
  delay(1000);
  if(my_id != compare_id){
    Serial.println("해당 ID가 아닙니다. 종료");
    return;
  }
  else{
    answer = Firebase.getString("uid/password");
    delay(1000);
    Serial.println("4개의 키를 입력 후, *을 눌러주세요");
  }
>>>>>>> 62f4df33184c2bde44582525acd8e6e99b185a99
}

void loop() {
  char inputkey = keypad.getKey();
  
  if(key_size >= 5){
    Serial.println("초과한 키를 입력하여 자동으로 Flush 됩니다.");
    bufferKey = "";
    key_size = 0;
  }
  
  else if(inputkey != NO_KEY){
    if(inputkey == '#'){
      bufferKey = "";
      key_size = 0;
      Serial.println("초기화입니다");
    }
    
    else if(inputkey == '*'){
       bool true_false = false;
       for(int i = 0; i < 4; i++){
        if(answer[i] != bufferKey[i]){
            true_false = true;
            break;
        }
       }
       if(true_false || key_size < 4){
          Serial.println("틀렸습니다");
          Firebase.setString("uid/uid_status", "open");
          if (Firebase.failed()) {
             Serial.print("setting /number failed:");
             Serial.println(Firebase.error());  
             return;
          }
<<<<<<< HEAD
          delay(1000);
=======
          digitalWrite(R, HIGH); // 닫힘
          delay(50);
>>>>>>> 62f4df33184c2bde44582525acd8e6e99b185a99
        }
        else{
          Serial.println("맞았습니다");
          Firebase.setString("uid/uid_status", "open");
          if (Firebase.failed()) {
             Serial.print("setting /number failed:");
             Serial.println(Firebase.error());  
             return;
          }
<<<<<<< HEAD
          delay(1000);
=======
          digitalWrite(R, LOW); // 열린 후 다시 닫힘
          delay(500);
          digitalWrite(R, HIGH); 
>>>>>>> 62f4df33184c2bde44582525acd8e6e99b185a99
        }
        bufferKey = "";
        key_size = 0;        
      }
      else{
        bufferKey += inputkey;
        Serial.print(bufferKey[key_size]);
        key_size++;
      }
    }
}
