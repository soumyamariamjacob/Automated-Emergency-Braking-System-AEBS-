#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TCS34725.h>

// Define pins for actuator control and sensor
#define ACTUATOR_PIN 9
#define SENSOR_PIN A0  // Assuming an analog sensor

// Threshold for collision detection (example value)
#define COLLISION_THRESHOLD 100  // Change based on sensor calibration

void setup() {
  pinMode(ACTUATOR_PIN, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(SENSOR_PIN);

  // Log sensor value for debugging
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);

  // Check for imminent collision
  if (sensorValue < COLLISION_THRESHOLD) {
    applyBrakes();
  } else {
    releaseBrakes();
  }

  delay(100);  // Adjust delay as needed for your system
}

void applyBrakes() {
  digitalWrite(ACTUATOR_PIN, HIGH);
  Serial.println("Brakes Applied");
}

void releaseBrakes() {
  digitalWrite(ACTUATOR_PIN, LOW);
  Serial.println("Brakes Released");
}