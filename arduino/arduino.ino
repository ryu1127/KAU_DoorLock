/*
 *  HTTPS 예제 수정 및 키패드 입력 후 matching 
 *  Firebase uid 로 데이터 매칭, 비밀번호 매칭했을때만 열리게하기
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

const char decryptedKey[27] = {
  'D','R','C','Z','A','E','X','P','O','S','L','J','G',
  'K','W','V','B','Q','Y','T','U','N','I','M','F','H'
};

byte rowPins[ROWS] = {D8, D7, D6, D5}; 
byte colPins[COLS] = {D4, D3, D0};

String bufferKey;
String answer;
String Serial_Number = ""; //uid 매칭을 위한 나의 id

int random_number = 0; //복호화를 위한 index
int answer_index = 0;
int key_size = 0; //숫자의 크기
bool input_flag = false; //*을 눌렀을 경우

int R = D2; // 솔레노이드를 위한

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

#define FIREBASE_HOST "*"
#define FIREBASE_AUTH "*"

#define WIFI_SSID "*"
#define WIFI_PASSWORD "*"

String line;

void setup() {
  Serial.begin(115200);
  pinMode(R, OUTPUT); //솔레노이드
  digitalWrite(R, LOW);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  String compare_id = Firebase.getString("lockers_info/custom_uid/serial_number");
  delay(1000);
  if(Serial_Number != compare_id){
    Serial.println("해당 ID가 아닙니다. 종료");
    return;
  }
  else{
    answer = Firebase.getString("lockers_info/custom_uid/password");
    Serial.println(answer);
    random_number = Firebase.getInt("lockers_info/custom_uid/random_num");
    answer = atoi(answer.c_str()) - decryptedKey[random_number] +'0';
    Serial.println(answer);
    delay(1000);
    Serial.println("4개의 키를 입력 후, *을 눌러주세요");
  }
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
          Firebase.setString("lockers_info/custom_uid/lock", "true");
          if (Firebase.failed()) {
             Serial.print("setting /number failed:");
             Serial.println(Firebase.error());  
             return;
          }
          digitalWrite(R, LOW); // relay off (for 1 Second)
          delay(1000);
        }
        else{
          Serial.println("맞았습니다");
          Firebase.setString("lockers_info/custom_uid/lock", "false");
          if (Firebase.failed()) {
             Serial.print("setting /number failed:");
             Serial.println(Firebase.error());  
             return;
          }
          digitalWrite(R, HIGH); // relay on (for 2 Second)
          delay(2000);
          digitalWrite(R, LOW); // relay on (for 2 Second)
        }
        bufferKey = "";
        key_size = 0;        
      }
      else{
        bufferKey += inputkey;
        Serial.print(bufferKey[key_size]);
        key_size++;
        answer = Firebase.getString("lockers_info/custom_uid/password");
        random_number = Firebase.getInt("lockers_info/custom_uid/random_num");
        answer = atoi(answer.c_str()) - decryptedKey[random_number] +'0';
        Serial.println(answer);
        delay(500);
      }
    }
}
