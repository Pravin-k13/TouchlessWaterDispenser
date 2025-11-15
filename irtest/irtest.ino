#define IR_SENSOR 2  // IR sensor output pin
#define LED 13        // LED pin

void setup() {
  pinMode(IR_SENSOR, INPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  if (digitalRead(IR_SENSOR) == LOW) {  // Object detected
    digitalWrite(LED, HIGH);  // Turn LED ON
  } else {
    digitalWrite(LED, LOW);   // Turn LED OFF
  }
}
