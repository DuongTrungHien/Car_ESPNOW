#include <esp_now.h>
#include <WiFi.h>
#include <SimpleKalmanFilter.h>
#define JX1 35
#define JY1 34
#define JX2 33
#define JY2 32
SimpleKalmanFilter bo_locX2(25, 25, 0.05);
SimpleKalmanFilter bo_locY2(25, 25, 0.05);
uint8_t receiverAddress[] = {0xd4, 0x8a, 0xfc, 0xcf, 0x96, 0x20}; // Địa chỉ MAC của bộ nhận

typedef struct struct_message {
    int joyX1;
    int joyY1;
    int joyX2;
    int joyY2;
} struct_message;

struct_message controlData;

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA); //Cho phép esp này kết nối với esp thu
  
   if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return; // Thoát nếu có lỗi
    }

  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  //receiverAddress: mảng có kích thước 6 byte chứa địa chỉ của Esp thu 
  //Hàm memcpy: sao chép 6 byte của mảng receiverAddress 
  peerInfo.channel = 0; //Đặt kênh là 0 để 
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return; // Thoát nếu có lỗi
    }
}

void loop() {
  controlData.joyX1 = analogRead(JX1);
  controlData.joyY1 = analogRead(JY1);
  controlData.joyX2 = bo_locX2.updateEstimate(analogRead(JX2));
  controlData.joyY2 = bo_locY2.updateEstimate(analogRead(JY2));
  Serial.print("Gia tri truoc loc:");
  Serial.println(analogRead(JX2));
  Serial.print("Gia tri sau loc:");
  Serial.println(controlData.joyY2);
  esp_now_send(receiverAddress, (uint8_t *) &controlData, sizeof(controlData));
  delay(100); // Giảm độ trễ để tránh việc gửi quá nhiều gói tin
}
