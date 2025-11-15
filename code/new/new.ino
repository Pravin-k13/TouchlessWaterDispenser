#include <Wire.h>
#include "Adafruit_VL53L0X.h"

#define IR_SENSOR 2             // IR sensor OUT pin
#define MOTOR_IN1 3             // Motor IN1
#define MOTOR_IN2 4             // Motor IN2
#define MOTOR_ENA 5             // Motor ENA (PWM)
#define WATER_FLOW_PIN 13       // Water Flow/LED Pin (LED near GND on Uno)

// Motor and container calibration
#define MOTOR_SPEED 1.667       // Speed in cm/sec
#define FRAME_SIZE 33           // Container Frame Size in cm

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
unsigned long startTime = 0;
unsigned long endTime = 0;
bool measuring = false;
bool returning = false;
bool waterFlowing = false;

// LiDAR variables
int distance;
float remainingSpace;

void setup() {
  pinMode(IR_SENSOR, INPUT);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_ENA, OUTPUT);
  pinMode(WATER_FLOW_PIN, OUTPUT);

  Serial.begin(9600);
  
  // Initialize LiDAR sensor
  while (!Serial) { delay(1); }

  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    Serial.println(F("Please check wiring and power"));
    while(1);
  }
  Serial.println(F("VL53L0X LiDAR sensor ready!"));
}

void loop() {
  int irStatus = digitalRead(IR_SENSOR);

  // Move while object is detected
  if (irStatus == LOW && !returning) {
    if (!measuring) {
      startTime = millis();
      measuring = true;
    }
    digitalWrite(MOTOR_IN1, HIGH);    // DOWNWARD movement
    digitalWrite(MOTOR_IN2, LOW);
    // digitalWrite(MOTOR_IN1, LOW);     // UPWARD movement
    // digitalWrite(MOTOR_IN2, HIGH);
    analogWrite(MOTOR_ENA, 255);
  }

  // Stop when object is no longer detected
  if (irStatus == HIGH && measuring) {
    measuring = false;

    // Immediately stop motor
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, LOW);
    analogWrite(MOTOR_ENA, 0);
    delay(100);  // Stabilize

    endTime = millis();  // Capture stop time
    unsigned long travelTime = endTime - startTime;
    float timeTaken = travelTime / 1000.0;
    float containerHeight = MOTOR_SPEED * timeTaken;

    Serial.print("Time Taken (Downward): ");
    Serial.print(timeTaken);
    Serial.println(" sec");

    Serial.print("Detected Container Height: ");
    Serial.print(containerHeight);
    Serial.println(" cm");

    remainingSpace = FRAME_SIZE - containerHeight + 1;
    Serial.print("Remaining Space: ");
    Serial.print(remainingSpace);
    Serial.println(" cm");

    delay(1000); // Pause before distance check
  }

  // Get LiDAR measurement
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  
  if (measure.RangeStatus != 4) {  // Check if measurement is valid
    distance = measure.RangeMilliMeter / 10; // Convert mm to cm
    Serial.print("LiDAR Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  } else {
    Serial.println("LiDAR out of range");
    distance = 200; // Default max range in cm
  }

  // Water flow control
  if (distance >= remainingSpace && !waterFlowing) {
    digitalWrite(WATER_FLOW_PIN, HIGH);  // Start water flow (LED ON)
    waterFlowing = true;
    Serial.println("Water Flow Started (LED ON)");
  }

  if (distance < remainingSpace && waterFlowing) {
    digitalWrite(WATER_FLOW_PIN, LOW);   // Stop water flow (LED OFF)
    waterFlowing = false;
    Serial.println("Water Flow Stopped (LED OFF)");
  }

  delay(500);  // Smoother sensor readings
}