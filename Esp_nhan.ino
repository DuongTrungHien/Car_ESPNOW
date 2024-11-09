#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>
Servo myServo1;
Servo myServo2;
#define enA  23
#define enB  5
#define servo1 14
#define servo2 13  
#define in1   22
#define in2   21
#define in3   19 
#define in4   18

int motorSpeed = 0;
int goc1=80;
int goc2=55;
typedef struct struct_message {
    int joyX1;
    int joyY1;
    int joyX2;
    int joyY2;
} struct_message;

struct_message controlData;

void OnDataRecv(const esp_now_recv_info *info, const uint8_t *data, int len) {
  memcpy(&controlData, data, sizeof(controlData));
  int joyY1 = controlData.joyY1;
  int joyX1 = controlData.joyX1;
  int joyX2 = controlData.joyX2;
  int joyY2 = controlData.joyY2;
  
  // Điều khiển tiến/lùi dựa trên giá trị joyY
  if (joyY1 <= 1700) { // Xe đi tiến
    motorSpeed = map(joyY1, 0, 1700, 180, 255);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);


  } else if (joyY1 >= 3200) { // Xe đi lùi
    motorSpeed = map(joyY1, 3200, 4095, 180, 255);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);


  } 

  // Điều khiển rẽ trái/phải dựa trên giá trị joyX
  else if (joyX1 <= 1700) { // Rẽ trái
    motorSpeed = map(joyX1, 0, 1700, 180, 255);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

  } else if (joyX1 > 3200) { // Rẽ phải
    motorSpeed = map(joyX1, 3200, 4095, 180, 255);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

  }
  else { // Dừng xe
    motorSpeed = 0;
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);

  }
    analogWrite(enA, motorSpeed);
    analogWrite(enB, motorSpeed);
  //Điều khiển servo1 từ 0-180
  if(abs(joyY2 -2050) > 700){
    if (joyY2 < 1700) {
        goc1 = map(joyY2, 0, 1700, 85, 130);
    } else if (joyY2 > 3200) {
        goc1 = map(joyY2,3200, 4095, 30, 60);
    }}
  //Điều khiển servo2 từ 0-180
 if(abs(joyX2 - 2050) > 700){
  if(joyX2 < 1700){
    goc2= map(joyX2, 0, 1700, 70, 75);
  }
  else if(joyX2 > 3200){
    goc2= map(joyX2, 3200, 4095, 30, 40);
  }
    }
      myServo1.write(goc1);
      myServo2.write(goc2);
}

void setup() {
  Serial.begin(9600);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  myServo1.attach(servo1);
  myServo2.attach(servo2);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
}
  esp_now_register_recv_cb(OnDataRecv);
}
void loop() {
  // Không có hành động trong vòng lặp chính; dữ liệu được xử lý trong OnDataRecv
}
