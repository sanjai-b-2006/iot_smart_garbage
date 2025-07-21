#include <ESP32Servo.h>


#define TRIG_PIN 13
#define ECHO_PIN 12
#define SERVO_PIN 14
#define GAS_SENSOR_PIN 34
#define LED_ORGANIC_PIN 27  
#define LED_LID_PIN 26      
Servo lidServo;


void setup() {
  Serial.begin(115200);
  lidServo.attach(SERVO_PIN);
  pinMode(TRIG_PIN, OUTPUT); pinMode(ECHO_PIN, INPUT);
  pinMode(GAS_SENSOR_PIN, INPUT);
  pinMode(LED_ORGANIC_PIN, OUTPUT);
  pinMode(LED_LID_PIN, OUTPUT);
  lidServo.write(0); 
}


void loop() {


  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(12);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  float distanceCM = duration * 0.034 / 2.0;


  // --- Gas sensor ---
  int gasValue = analogRead(GAS_SENSOR_PIN);
  bool isOrganic = gasValue > 600;


  // --- Lid logic & LEDs ---
  if(distanceCM > 0 && distanceCM <= 15) {
    // Object close: open lid and show status
    lidServo.write(90); // Open lid
    digitalWrite(LED_LID_PIN, HIGH); // Lid open LED ON
    if(isOrganic) {
      digitalWrite(LED_ORGANIC_PIN, HIGH);  // Green ON for organic
    } else {
      digitalWrite(LED_ORGANIC_PIN, LOW);   // Green OFF for non-organic
    }
    delay(1200);      // Lid stays open
    lidServo.write(0);  // Close lid
    digitalWrite(LED_LID_PIN, LOW); // Lid closed LED OFF
    digitalWrite(LED_ORGANIC_PIN, LOW); // Reset after sorting
    delay(500);
  } else {
    lidServo.write(0);  // keep closed
    digitalWrite(LED_LID_PIN, LOW); // Lid closed
    digitalWrite(LED_ORGANIC_PIN, LOW); // No indication
  }
  delay(500);
} 
