#include <ESP32Servo.h>
#include "servo.h"

class ServoManager {
private:
  
  int backspinPositions = 2;
  int digits[4];

public:
  ServoManager() {}

  void setup() {
    for (int i = 0; i < numServos; i++) {
      servo[i].attach(servoPins[i]);
    }
    for (int i = 0; i < numServos; i++) {
        servo[i].write(servoPos[i][0]);
    }
  }

  void SetNumber(int number) 
  {
    

    // Jede Ziffer von der Zahl extrahiert
    digits[0] = (number / 1000) % 10; // Thousands place
    digits[1] = (number / 100) % 10;  // Hundreds place
    digits[2] = (number / 10) % 10;   // Tens place
    digits[3] = number % 10;          // Units place

    // Die Servos jeweils zu den Positionen fahren
for (int i = 0; i < numServos; i++) 
  {
     
    servo[i].write(servoPos[i][digits[i - backspinPositions]]);
    delay(100);
    //Servos werden von links nach rechts links angesteuert
    servo[i].write(servoPos[i][digits[i]]);
    delay(200); // Delay between servo movements
    
    Serial.print("Servo Nr.");
    Serial.print(i+1);
    Serial.print(" = ");
    Serial.print(servoPos[i][digits[i]]);
    Serial.print(" is Position ");
    Serial.println(digits[i]);    
  }
  }
};