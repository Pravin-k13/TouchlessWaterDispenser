#include <Wire.h>
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(115200);
  
  // Wait until serial port opens (for native USB)
  while (!Serial) { delay(1); }

  Serial.println("Initializing VL53L0X sensor...");
  
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    Serial.println(F("Please check:"));
    Serial.println(F("1. Sensor wiring (SDA=A4, SCL=A5)"));
    Serial.println(F("2. Library is installed (Adafruit_VL53L0X)"));
    Serial.println(F("3. Sensor is powered (3.3V-5V)"));
    while(1);
  }
  Serial.println(F("VL53L0X sensor ready!"));
  Serial.println(F("---------------------"));
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  
  lox.rangingTest(&measure, false); // Get measurement data

  if (measure.RangeStatus != 4) {  // Check if measurement is valid
    Serial.print("Distance: "); 
    Serial.print(measure.RangeMilliMeter);
    Serial.println(" mm");
  } else {
    Serial.println("Out of range (>2m)");
  }
  
  delay(2000); // 2 second delay between measurements
  Serial.println(); // Add blank line between readings
}