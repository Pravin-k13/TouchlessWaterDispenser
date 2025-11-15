const int pumpPin = 8;  // Connect your motor or control pin here

void setup() {
  pinMode(pumpPin, OUTPUT);  // Set pin as output
}

void loop() {
  digitalWrite(pumpPin, HIGH);  // Turn ON (HIGH)
  delay(5000);                  // Wait for 5 seconds

  digitalWrite(pumpPin, LOW);   // Turn OFF (LOW)
  delay(5000);                  // Wait for 5 seconds
}
