#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <ArduinoJson.h>

const char* ssid = "RaghavS";
const char* password = "gareeb123";

// Target Unity machine
const char* targetIP = "192.168.174.180";
const int targetPort = 5000;

WiFiUDP Udp;

// Create the sensor object
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(9600);
  
  // Initialize I2C (D1 is SCL, D2 is SDA on NodeMCU/Wemos)
  Wire.begin(); 

  // Wait for Serial Monitor
  while (!Serial) { delay(1); }

  // 1. Connect to WiFi
  Serial.println("\nConnecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  // 2. Initialize VL53L0X Sensor
  Serial.println("Booting VL53L0X...");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1); // Stop here if sensor not found
  }
  Serial.println(F("VL53L0X ready"));
}

void loop() {
  // --- 1. READ SENSORS ---

  // Read Potentiometer
  int potVal = analogRead(A0);

  // Read VL53L0X
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); // pass 'true' to get debug data printout

  int distanceVal = -1; // Default to -1 if out of range
  if (measure.RangeStatus != 4) {  // Phase failures have incorrect data
    distanceVal = measure.RangeMilliMeter;
  } else {
    // Serial.println(" out of range ");
    distanceVal = 8190; // Common value for "too far"
  }

  // --- 2. CREATE JSON ---
  
  // Calculate size: We have 2 integers. 96 bytes is usually plenty for this.
  StaticJsonDocument<200> doc;
  
  doc["pot"] = potVal;
  doc["dist"] = distanceVal;

  // Serialize JSON to a string buffer
  char buffer[200];
  serializeJson(doc, buffer);

  // --- 3. SEND UDP ---
  
  Udp.beginPacket(targetIP, targetPort);
  Udp.print(buffer); // Sends: {"pot":512,"dist":150}
  Udp.endPacket();

  // Debug print to Serial Monitor (Optional)
  Serial.println(buffer);

  // --- 4. DELAY ---
  delay(16); // ~60fps
}