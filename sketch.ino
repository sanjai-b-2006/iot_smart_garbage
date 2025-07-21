#include <WiFi.h>
#include <ESP32Servo.h>
#include "HX711.h"

// Pin Definitions
#define TRIG_PIN 13     // Ultrasonic Trigger
#define ECHO_PIN 12     // Ultrasonic Echo
#define SERVO_PIN 14    // Sorting mechanism
#define GAS_SENSOR_PIN 34  // MQ2 analog (A2)
#define HX711_DOUT 26   // Weight sensor
#define HX711_SCK 25
Serial.println("Setup started");

// Sensor & Control
Servo sortServo;
HX711 scale;

// WiFi credentials (for demo, won't connect in Wokwi)
const char* ssid     = "Wokwi-GUEST";
const char* password = "";

// State Variables
long duration;
float distanceCM, weightGrams;
int gasReading, wasteType; // 0: Unknown, 1: Plastic, 2: Metal, 3: Organic

void setup() {
  Serial.begin(115200);
  sortServo.attach(SERVO_PIN);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(GAS_SENSOR_PIN, INPUT);
  scale.begin(HX711_DOUT, HX711_SCK);
  sortServo.write(90); // Home Position

  // WiFi init (will print to Serial instead in Wokwi)
  WiFi.begin(ssid, password);

  Serial.println("Smart Bin System Startup");
}

void loop() {
  // Fill-Level: Ultrasonic
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(15);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distanceCM = duration * 0.034 / 2;

  // Weight: HX711 (simulate: set zero offset in real HW)
  long weightVal = scale.get_units(1);
  weightGrams = weightVal / 100.0; // Adjust calibration factor as per your sensor

  // Gas Sensor (Organics Detection)
  gasReading = analogRead(GAS_SENSOR_PIN);

  // "Classify" Waste Type (mock-up: use thresholds, buttons, or code switches)
  // In real version, add button for "user adds plastic/metal/organic", or use external AI
  wasteType = 0;
  if (gasReading > 600) { // Simulated high MQ for organic
    wasteType = 3;
  } else if (weightGrams > 100) { // Simulate metal if heavy
    wasteType = 2;
  } else { // Default: plastic/paper
    wasteType = 1;
  }

  // Sorting Mechanism: Servo Position
  if (wasteType == 3) sortServo.write(30);        // Organic
  else if (wasteType == 2) sortServo.write(150);  // Metal
  else if (wasteType == 1) sortServo.write(90);   // Plastic/paper

  // WiFi/IoT: Simulate sending data to cloud
  Serial.print("Ultrasonic Distance (cm): "); Serial.println(distanceCM);
  Serial.print("Weight Sensor (g): "); Serial.println(weightGrams);
  Serial.print("Gas Sensor Value: "); Serial.println(gasReading);

  String typeStr = (wasteType == 1) ? "Plastic/Paper" : (wasteType == 2) ? "Metal" : "Organic";
  Serial.print("Detected Waste Type: "); Serial.println(typeStr);

  // Simulate status to MQTT/cloud dashboard
  Serial.println("Sending data to Cloud Dashboard...");
  // In real implementation: publish to MQTT server here

  // Fill-level Alert
  if (distanceCM < 10 || weightGrams > 900) {
    Serial.println("!! ALERT: Bin almost full, collection required !!");
  }

  delay(2000);  // Repeat every 2 seconds (simulate periodic scan)
}
