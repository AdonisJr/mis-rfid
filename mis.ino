#include <SPI.h>
#include <MFRC522.h>
#include "WiFiS3.h"

// Wi-Fi credentials
const char* ssid = "adonis";
const char* password = "123456789";

// Laravel API server
const char* serverIP = "10.238.161.131";
const uint16_t serverPort = 8000;

// RC522 Pins
#define SS_PIN 10
#define RST_PIN 7
MFRC522 rfid(SS_PIN, RST_PIN);

// Wi-Fi client
WiFiClient client;

// LEDs & BUTTON
int REDLED = 6;
int GREENLED = 5;
int BUTTON = 4;
int YELLOWLED = 3;

// ---- Toggle button variables ----
bool yellowState = false;     // Current LED state
bool lastButtonState = HIGH;  // Previous button state


// ---------------- WIFI CHECK FUNCTION ----------------
void checkWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi LOST! Reconnecting...");

    digitalWrite(GREENLED, LOW);  // green off
    digitalWrite(REDLED, HIGH);   // red ON

    WiFi.begin(ssid, password);
    unsigned long startAttempt = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 7000) {
      delay(300);
      Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWiFi reconnected!");
      digitalWrite(REDLED, LOW);
      digitalWrite(GREENLED, HIGH);
    } else {
      Serial.println("\nStill not connected...");
    }
  }
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(REDLED, OUTPUT);
  pinMode(GREENLED, OUTPUT);
  pinMode(YELLOWLED, OUTPUT);

  pinMode(BUTTON, INPUT_PULLUP);  // BUTTON active LOW

  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(REDLED, HIGH);
    digitalWrite(GREENLED, LOW);
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  digitalWrite(REDLED, LOW);
  digitalWrite(GREENLED, HIGH);
}

void loop() {

  // ---- TOGGLE BUTTON LOGIC ----
  int reading = digitalRead(BUTTON);

  // Button pressed (transition from HIGH → LOW)
  if (lastButtonState == HIGH && reading == LOW) {
    yellowState = !yellowState;  // Toggle
    digitalWrite(YELLOWLED, yellowState ? HIGH : LOW);
    delay(200);  // simple debounce
  }

  lastButtonState = reading;

  // ---- WIFI CHECK ----

  checkWiFi();

  // ---- RFID SCAN ----
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  // Build UID
  String uidStr = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (i > 0) uidStr += ":";
    if (rfid.uid.uidByte[i] < 0x10) uidStr += "0";
    uidStr += String(rfid.uid.uidByte[i], HEX);
  }
  uidStr.toUpperCase();
  Serial.println("Scanned UID: " + uidStr);

  // JSON payload
  String json = "{\"rfid\":\"" + uidStr + "\"}";

  // Send to API
  if (client.connect(serverIP, serverPort)) {
    client.println("POST /api/scan HTTP/1.1");
    client.println("Host: 10.238.161.131:8000");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println((int)json.length());
    client.println();
    client.println(json);

    unsigned long timeout = millis();
    while (client.connected() && !client.available()) {
      if (millis() - timeout > 5000) break;
      delay(10);
    }

    while (client.available()) {
      String line = client.readStringUntil('\n');
      Serial.println(line);
    }

    client.stop();
    Serial.println("POST complete.\n");
  } else {
    Serial.println("Connection to server failed");
  }

  rfid.PICC_HaltA();
  delay(1000);
}
