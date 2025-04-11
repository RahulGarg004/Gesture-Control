#include <Wire.h>
#include <ESP8266WiFi.h>
#include <MPU6050.h>

const char* ssid = "YOUR_SSID";         // Same as used in ESP32
const char* password = "YOUR_PASSWORD"; // Same as used in ESP32
const char* host = "192.168.4.1";       // Default IP of ESP32 in AP mode

MPU6050 mpu;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();

  // Connect to ESP32 AP
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to ESP32");
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  String command = "";

  if (ay > 10000) command = "F";        // Tilt forward
  else if (ay < -10000) command = "B";  // Tilt backward
  else if (ax > 10000) command = "L";   // Tilt left
  else if (ax < -10000) command = "R";  // Tilt right
  else command = "S";                   // Stay still / Stop

  Serial.println("Gesture: " + command);

  if (client.connect(host, 80)) {
    client.print(command + "\n");
    client.stop();
  }

  delay(200);  // Adjust based on responsiveness
}
