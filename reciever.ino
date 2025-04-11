#include <WiFi.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Motor A - Left motor
#define IN1 18
#define IN2 19
#define ENA 5

// Motor B - Right motor
#define IN3 16
#define IN4 17
#define ENB 4

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  // Motor pins setup
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Start Access Point
  WiFi.softAP(ssid, password);
  server.begin();
  Serial.println("ESP32 ready and waiting for gesture data...");
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String command = client.readStringUntil('\n');
    command.trim();
    Serial.println("Command: " + command);

    if (command == "F") moveForward();
    else if (command == "B") moveBackward();
    else if (command == "L") turnLeft();
    else if (command == "R") turnRight();
    else stopMotors();

    client.stop();
  }
}

// Both motors forward
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
}

// Both motors backward
void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
}

// Left turn: right motor ON only
void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 200);
}

// Right turn: left motor ON only
void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 200);
  analogWrite(ENB, 0);
}

// Stop all motors
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
