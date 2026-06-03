#include <GL868_ESP32.h>

// ======================================
// CONFIG
// ======================================

#define DEVICE_ID "your_device_id"
#define API_KEY "your_api_key"

#define BUTTON_PIN 4

const char* phoneNumber = "+91----------";

// ======================================

bool lastButtonState = HIGH;
bool callActive = false;
bool audioConfigured = false; // FIX: Prevents the endless command loop

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.println("GL868 Voice System");

  // Initialize library
  GeoLinker.begin(DEVICE_ID, API_KEY);

  // Power modem
  Serial.println("Powering modem...");
  if (!GeoLinker.modem.powerOn()) {
    Serial.println("Modem power failed!");
    while (1) delay(1000);
  }

  Serial.println("Waiting for network...");
  while (!GeoLinker.gsm.waitNetworkRegistration(10000)) {
    Serial.println("Searching network...");
  }
  Serial.println("Network connected");

  // Global Audio Setup
  Serial2.println("AT+CHFA=1");
  delay(100);
  Serial2.println("AT+CLVL=90");
  delay(100);
  Serial2.println("AT+CMIC=0,12");
  delay(100);
  Serial2.println("AT+FMMUTE=0");
  delay(100);
  Serial2.println("AT+SIDET=1");
  delay(100);

  Serial.println("Ready");
}

void loop() {

  // ======================================
  // BUTTON PRESS -> VOICE CALL
  // ======================================
  bool currentState = digitalRead(BUTTON_PIN);

  if (lastButtonState == HIGH && currentState == LOW && !callActive) {
    delay(50);

    if (digitalRead(BUTTON_PIN) == LOW) {
      Serial.println("Executing Voice Dial Sequence...");

      Serial2.println("AT+CHFA=1");
      delay(100);
      Serial2.println("AT+CLVL=90");
      delay(100);
      Serial2.println("AT+CMIC=0,12");
      delay(100);
      Serial2.println("AT+FMMUTE=0");
      delay(100);

      // Start voice call
      Serial2.print("ATD");
      Serial2.print(phoneNumber);
      Serial2.println(";");

      Serial.print("Dialing: ATD");
      Serial.print(phoneNumber);
      Serial.println(";");

      callActive = true;
      audioConfigured = false; // Reset for the new outgoing call
    }
  } 
  lastButtonState = currentState;

  // ======================================
  // MODEM RESPONSE HANDLING
  // ======================================
  while (Serial2.available()) {
    String response = Serial2.readStringUntil('\n');
    response.trim();

    if (response.length() > 0) {
      Serial.println(response);

      // --- INCOMING CALL ---
      if (response.indexOf("RING") >= 0) {
        Serial.println("Incoming Call... Answering");
        delay(1000);
        Serial2.println("ATA");
        
        callActive = true;
        audioConfigured = false; // Reset for the incoming call
      }

      // --- OUTGOING/INCOMING CALL CONNECTED (RUNS ONLY ONCE) ---
      // We look explicitly for connection confirmations and ensure it hasn't run yet
      if (callActive && !audioConfigured && 
          (response.indexOf("MO CONNECTED") >= 0 || response.indexOf("CONNECT") >= 0)) {

        Serial.println("[Audio Patch] Call connected. Configuring audio routing ONCE.");
        audioConfigured = true; // Lock the block so it cannot loop

        delay(300);
        Serial2.println("AT+CHFA=1");
        delay(100);
        Serial2.println("AT+CLVL=90");
        delay(100);
        Serial2.println("AT+CMIC=0,12");
        delay(100);
        Serial2.println("AT+FMMUTE=0");
        delay(100);
        Serial2.println("AT+SIDET=1");
        delay(100);
      }

      // --- CALL TERMINATED ---
      if (response.indexOf("NO CARRIER") >= 0 ||
          response.indexOf("BUSY") >= 0 ||
          response.indexOf("NO ANSWER") >= 0) { 

        Serial.println("Call Disconnected.");
        callActive = false;
        audioConfigured = false;
      }
    }
  }

  // Only run updates when idle
  if (!callActive) {
    GeoLinker.update();
  }

  delay(20);
}