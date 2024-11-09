#include <WiFi.h>

void setup() {
  Serial.begin(115200);  
  String mac = WiFi.macAddress();  // Lấy địa chỉ MAC của esp32 thu
  Serial.print("Địa chỉ MAC: ");
  Serial.println(mac); // In địa chỉ MAC
}

void loop() {

}


