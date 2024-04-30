#include <ESP32Servo.h>
#include "servo.h"

class ServoManager
{
private:
  int backspinPositions = 2;
  int digits[4];

public:
  ServoManager() {}

  void setup()
  {
    for (int i = 0; i < numServos; i++)
    {
      servo[i].attach(servoPins[i]);
    }
    for (int i = 0; i < numServos; i++)
    {
      servo[i].write(servoPos[i][0]);
    }
  }

  void SetNumber(int number)
  {
    // Überprüfung ob die Zahl 4-stellig ist
    if (number < 0 || number > 9999)
    {
      Serial.println("übergebene Zahl ungültig || nicht zwischen 0 - 9999");
    }
    else
    {

      // Jede Ziffer von der Zahl extrahiert
      digits[0] = (number / 1000) % 10; // Tausenderstelle
      digits[1] = (number / 100) % 10;  // Hunderterstelle
      digits[2] = (number / 10) % 10;   // Zehnerstelle
      digits[3] = number % 10;          // Einerstelle

      // Die Servos jeweils zu den Positionen fahren
      for (int i = 0; i < numServos; i++)
      {
        // Überprüfung ob der Backspin unter 0 geht
        if (0 > i - backspinPositions)
        {
          // Vordefinierter Backspin
          servo[i].write(servoPos[i][digits[i - backspinPositions]]);
        }
        else
        {
          // Backspin starr auf die Position 0 gesetzt
          servo[i].write(0);
        }
        delay(200);

        // Servos werden von links nach rechts angesteuert
        // Servo Nr.1 = links
        // Servo Nr.4 = rechts
        servo[i].write(servoPos[i][digits[i]]);
        delay(200); // Delay between servo movements

        Serial.print("Servo Nr.");
        Serial.print(i + 1);
        Serial.print(" = ");
        Serial.print(servoPos[i][digits[i]]);
        Serial.print(" ist Position ");
        Serial.println(digits[i]);
      }
    }
  }
};