#include <ESP32Servo.h>
#include "servo.h"

class ServoManager {

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
};