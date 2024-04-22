#ifndef SERVO_H
#define SERVO_H

#include <ESP32Servo.h>

extern const int numServos;
extern Servo servo[];
extern const int servoPins[];
extern int servoPos[4][10];

#endif
