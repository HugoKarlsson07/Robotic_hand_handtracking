/*
 * Name: robotic_hand.ino
 * Author: Hugo Karlsson
 * Date: 2025-04-11
 *description:this program is used to control a robotic hand by reciving data from a python program that uses serial comunication to send data to the arduino.
 * The data is sent in the format [finger1,finger2,finger3,finger4,finger5,servoX,servoY]
 */

#include <Servo.h> // Inkluderar Servo-biblioteket som används för att styra servomotorer

// Skapar tre servo-objekt: ett för X-axel, ett för Y-axel och ett för "handen"
Servo servoX;
Servo servoY;
Servo servoH;

// Array som innehåller de digitala pin-nummer servomotorerna är kopplade till
int servoPins[] = {3, 4, 5};

void setup()
{
    Serial.begin(9600); // Startar seriell kommunikation med en baudrate på 9600 bps

    // Kopplar varje servo till motsvarande pin enligt servoPins-arrayen
    servoX.attach(servoPins[0]);
    servoY.attach(servoPins[1]);
    servoH.attach(servoPins[2]);

    // Sätter startposition för servona
    servoX.write(90); // X-axeln i mittenläge
    servoY.write(90); // Y-axeln i mittenläge
    servoH.write(0);  // Handen stängd
    delay(500);       // Väntar en halv sekund för att servona ska hinna röra sig
}

void loop()
{
    handleSerialInput(); // Kontrollerar om ny seriell data har kommit in
}

// Funktion för att hantera inkommande data från seriell port
void handleSerialInput()
{
    // Om det finns data tillgänglig att läsa
    if (Serial.available() > 0)
    {
        // Kontrollera att meddelandet börjar med [
        if (Serial.read() == '[')
        {
            int values[7];         // Array för att lagra de inkommande värdena: 5 fingrar + 2 servo-axlar
            bool validData = true; // Flagga för att avgöra om inkommande data är giltig

            // Läser 7 värden från meddelandet
            for (int i = 0; i < 7; i++)
            {
                values[i] = Serial.parseInt(); // Läser ett heltal

                // Kontroll för att se om det som lästs är felaktigt (dvs. ett oväntat tecken)
                if (values[i] == 0 && Serial.peek() != ',' && Serial.peek() != ']')
                {
                    validData = false; // Om data inte verkar korrekt, markera det
                    break;             // Avbryt loopen
                }

                // Hoppar över kommatecken (utom efter sista värdet)
                if (i < 6)
                    Serial.read();
            }

            // Om datan var giltig och meddelandet avslutas med ]
            if (validData && Serial.read() == ']')
            {
                // Begränsar servovärden till tillåtna värden (0-180 grader)
                int servoX_val = constrain(values[5], 0, 180);
                int servoY_val = constrain(values[6], 0, 180);

                // Flytta X- och Y-servon till de angivna positionerna
                servoX.write(servoX_val);
                servoY.write(servoY_val);

                // Kontroll: om alla fem fingervärden är 0 (dvs alla fingrar "upp")
                bool allFingersUp = (values[0] == 0 && values[1] == 0 && values[2] == 0 && values[3] == 0 && values[4] == 0);

                // Baserat på fingrarnas tillstånd, öppna eller stäng handen
                if (allFingersUp)
                {
                    servoH.write(90); // Öppna handen (servo i 90 grader)
                }
                else
                {
                    servoH.write(0); // Stäng handen (servo i 0 grader)
                }
            }
        }
    }
}
