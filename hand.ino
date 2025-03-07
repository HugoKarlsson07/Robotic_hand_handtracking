#include <Arduino.h>
#include <Servo.h>

Servo servoX; // Servo för X-axeln
Servo servoY; // Servo för Y-axeln

int ledPins[] = {8, 9, 10, 11, 12}; // LED för fingrarna
int servoPins[] = {3, 4};           // Servopins

void setup()
{
    Serial.begin(9600); // Starta seriell kommunikation

    // Initiera LED-pins som output
    for (int i = 0; i < 5; i++)
    {
        pinMode(ledPins[i], OUTPUT);
    }

    // Initiera servon
    servoX.attach(servoPins[0]);
    servoY.attach(servoPins[1]);

    // Ställ servon i startposition (45 grader)
    servoX.write(90);
    servoY.write(90);
    delay(500);
}

void loop()
{
    if (Serial.available() > 0)
    {
        // Kontrollera att vi får hela meddelandet
        if (Serial.read() == '[')
        {
            int values[7]; // 5 fingrar + 2 servon
            bool validData = true;

            for (int i = 0; i < 7; i++)
            {
                values[i] = Serial.parseInt();
                if (values[i] == 0 && Serial.peek() != ',' && Serial.peek() != ']')
                {
                    validData = false; // Om felaktig data, ignorera
                    break;
                }
                if (i < 6)
                    Serial.read(); // Läser bort kommatecken
            }

            if (validData && Serial.read() == ']')
            {
                // Sätt finger-LEDs
                for (int i = 0; i < 5; i++)
                {
                    digitalWrite(ledPins[i], values[i] ? HIGH : LOW);
                }

                // Begränsa servovärden till 0-180 grader
                int servoX_val = constrain(values[5], 0, 180);
                int servoY_val = constrain(values[6], 0, 180);

                // Skicka servovärden till servona
                servoX.write(servoX_val);
                servoY.write(servoY_val);

                // Debug-utskrift till Serial Monitor
                Serial.print("Fingers: ");
                for (int i = 0; i < 5; i++)
                {
                    Serial.print(values[i]);
                    Serial.print(" ");
                }
                Serial.print(" | Servo X: ");
                Serial.print(servoX_val);
                Serial.print(" | Servo Y: ");
                Serial.println(servoY_val);
            }
        }
    }
}
