const int solenoidPin = 2;  // Digital pin to which the solenoid valve is connected

void setup() {
  pinMode(solenoidPin, OUTPUT); // Set the solenoidPin as an output
}

void loop() {
  // Open the solenoid valve (assuming LOW activates the valve, check your datasheet)
  digitalWrite(solenoidPin, LOW);
  delay(3000);  // Open the valve for 3 seconds
  
  // Close the solenoid valve
  digitalWrite(solenoidPin, HIGH);
  delay(3000);  // Close the valve for 3 seconds
}
