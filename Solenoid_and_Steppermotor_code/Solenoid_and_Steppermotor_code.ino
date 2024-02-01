#include <AccelStepper.h>

// Define the connections for solenoid valves
const int openSolenoidPin = 4;  // Digital pin for opening solenoid valve
const int closeSolenoidPin = 5; // Digital pin for closing solenoid valve

// Define the connections for the stepper motor
#define PUL_PIN 2  // Connect PUL+ to Arduino digital pin 2
#define DIR_PIN 3  // Connect DIR+ to Arduino digital pin 3
#define DIR_INVERT true  // Set to 'true' if the motor direction needs to be inverted

// Create an AccelStepper object
AccelStepper stepper(AccelStepper::DRIVER, PUL_PIN, DIR_PIN);

void setup() {
  // Set the solenoid valve pins as outputs
  pinMode(openSolenoidPin, OUTPUT);
  pinMode(closeSolenoidPin, OUTPUT);

  // Set the maximum speed and acceleration for the stepper motor
  stepper.setMaxSpeed(1000);  // Adjust this value as needed
  stepper.setAcceleration(500); // Adjust this value as needed

  // Set the direction inversion if needed
  stepper.setPinsInverted(DIR_INVERT, false, false);

  // Set the initial position (optional)
  stepper.setCurrentPosition(0);

  // You can also set the motor's enable pin (ENBL) if needed
  // pinMode(ENBL_PIN, OUTPUT);
  // digitalWrite(ENBL_PIN, LOW); // Enable the motor
}

void loop() {
  // Open the solenoid valve
  /*
  digitalWrite(openSolenoidPin, LOW);
  digitalWrite(closeSolenoidPin, HIGH);
  delay(3000);  // Open the valve for 3 seconds

  // Close the solenoid valve
  digitalWrite(openSolenoidPin, HIGH);
  digitalWrite(closeSolenoidPin, LOW);
  delay(3000);  // Close the valve for 3 seconds
    */
  // Move the motor a specified number of steps
  stepper.moveTo(905);  // Adjust the number of steps as needed

  // Run the motor until it reaches the target position
  while (stepper.isRunning()) {
    stepper.run();
  }

  // Reverse the direction and move back to the starting position
  stepper.moveTo(0);
  while (stepper.isRunning()) {
    stepper.run();
  }

  // You can repeat this loop or implement your desired logic here
}
void SerialCommunication() {

    if (Serial.available() > 0) {
    char command = Serial.read();

    if (command == 'Start Pump 1') {
        
        digitalWrite(openSolenoidPin, LOW);
        digitalWrite(closeSolenoidPin, HIGH);
        
    } else if (command == 'Start Pump 2') {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}