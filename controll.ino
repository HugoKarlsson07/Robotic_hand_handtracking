#include <Arduino.h>
#include <Servo.h>

Servo servoX;                // Servo för X-axeln
Servo servoY;                // Servo för Y-axeln
Servo servoH;                // Servo för handen
int servoPins[] = {3, 4, 5}; // Servopins

void setup()
{
    Serial.begin(9600); // Starta seriell kommunikation

    // Initiera servon
    servoX.attach(servoPins[0]);
    servoY.attach(servoPins[1]);
    servoH.attach(servoPins[2]);

    // Ställ servon i startposition
    servoX.write(90);
    servoY.write(90);
    servoH.write(0);
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
                // Begränsa servovärden till 0-180 grader
                int servoX_val = constrain(values[5], 0, 180);
                int servoY_val = constrain(values[6], 0, 180);

                // Skicka servovärden till servona
                servoX.write(servoX_val);
                servoY.write(servoY_val);

                // Kolla om alla fingrar är upp (dvs. alla fem första värden är 1)
                bool allFingersUp = (values[0] == 1 && values[1] == 1 && values[2] == 1 && values[3] == 1 && values[4] == 1);

                // Ställ in servoH baserat på fingersignal
                if (allFingersUp)
                {
                    servoH.write(90); // Öppen hand
                }
                else
                {
                    servoH.write(0); // Stängd hand
                }
            }
        }
    }
}
