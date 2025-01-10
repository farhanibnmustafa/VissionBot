#include "Arduino.h"
#include "DFPlayerMini_Fast.h"
// Ultrasonic sensor pins
#define TRIG_PIN 23 // Ultrasonic sensor trigger pin
#define ECHO_PIN 22 // Ultrasonic sensor echo pin

// Hardware Serial for DFPlayer Mini
HardwareSerial mySerial(1); // UART2
DFPlayerMini_Fast myDFPlayer;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize Ultrasonic Sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Initialize DFPlayer Mini
  mySerial.begin(9600, SERIAL_8N1, 16, 17); // RX=GPIO16, TX=GPIO17
  Serial.println("Initializing DFPlayer...");
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("DFPlayer initialization failed!");
    while (true); // If there's an error, stop everything
  }
  myDFPlayer.volume(25); // Set volume level (0~30)
  Serial.println("Setup completed.");
}

void loop() {
  // Measure distance using Ultrasonic Sensor
  long distance = measureDistance();
  Serial.print("Distance: ");
  Serial.println(distance);

  // Play audio based on distance ranges
  if (distance > 0 && distance <= 15) {
    Serial.println("Object detected within 15 cm!");
    myDFPlayer.play(1); // Play 0001.mp3
    delay(3000); // Wait for the audio to finish
  } else if (distance > 15 && distance <= 25) {
    Serial.println("Object detected between 15 and 25 cm!");
    myDFPlayer.play(2); // Play 0002.mp3
    delay(3000); // Wait for the audio to finish
  } else if (distance > 25 && distance <= 65) {
    Serial.println("Object detected between 25 and 65 cm!");
    myDFPlayer.play(3); // Play 0003.mp3
    delay(3000); // Wait for the audio to finish
  } else {
    Serial.println("No object detected.");
    delay(500); // Small delay to avoid continuous measurement
  }
}

// Function to measure distance using Ultrasonic Sensor
long measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo pulse duration
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate distance in cm
  long distance = (duration * 0.034) / 2;
  return distance;
}
