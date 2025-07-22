#include <Arduino.h>
#include <Servo.h>
#include "HX711.h"

// Use valid UNO pins

const int trigPin = 5;
const int echoPin = 6;
const int servoPin = 7;
const int gasSensorPin = A0;    // analog pin A0
const int redLedPin = 10;       // digital pin 10
const int greenLedPin = 11;     // digital pin 11

// HX711 to UNO digital pins
const int HX711_DOUT = 8;       // digital pin 8
const int HX711_SCK = 9;        // digital pin 9


Servo servo;
HX711 scale;

long duration;
int distance;
int gasValue;

// Thresholds (adjust as needed)
int gasThreshold = 350;
int distanceThreshold = 50;

void setup() {
  Serial.begin(9600);

  servo.attach(servoPin);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(gasSensorPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);

  scale.begin(HX711_DOUT, HX711_SCK);

  Serial.println("Smart Bin System Initialized");
}

void measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
}

void loop() {
  measureDistance();

  if (distance < distanceThreshold) {
    servo.write(180);
    Serial.println("Bin lid open. Dispose waste.");

    gasValue = analogRead(gasSensorPin);
    Serial.print("Gas sensor value: ");
    Serial.println(gasValue);

    if (gasValue > gasThreshold) {
      Serial.println("Waste type: Organic");
      digitalWrite(redLedPin, HIGH);
      digitalWrite(greenLedPin, LOW);
    } else {
      Serial.println("Waste type: Recyclable");
      digitalWrite(redLedPin, LOW);
      digitalWrite(greenLedPin, HIGH);
    }

    // Read weight from load cell
    if (scale.is_ready()) {
      long weight = scale.read();
      Serial.print("Weight (raw): ");
      Serial.println(weight);
    } else {
      Serial.println("HX711 not ready.");
    }

  } else {
    servo.write(0);
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, LOW);
    Serial.println("Bin lid closed.");
  }

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(1000);
}
