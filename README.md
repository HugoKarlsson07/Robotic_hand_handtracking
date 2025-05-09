Robotic Hand Handtracking
This project features a 3D-printed robotic arm controlled by real-time hand gestures using computer vision. It leverages Python and OpenCV for hand tracking and communicates with an Arduino Uno R4 WiFi to actuate servo motors.​

Youtube video
https://youtu.be/Jrf-QRCd0Hs?feature=shared

Overview
Control Method: Hand gesture recognition via webcam to a arduino uno r4 via serial


Hardware:

3D-printed robotic arm (PLA, printed on a Prusa MK4S).​


Standard servo motors.​

Arduino Uno R4 WiFi.​

Software:

Python script (hand.py) for hand detection and gesture interpretation.​


Arduino sketch (controll.ino) for servo control based on received commands.​



Serial communication between Python and Arduino for responsive control.​
GitHub

Modular design allowing for customization and expansion.​

Getting Started
Prerequisites
Hardware:

3D-printed robotic arm with servo motors.​

Arduino Uno R4 WiFi.​

Webcam for hand gesture input.​

Software:

Python 3.11

Arduino IDE​

Python packages: opencv-python, serial​

Installation
Clone the Repository:

bash
Kopiera
Redigera
git clone https://github.com/HugoKarlsson07/Robotic_hand_handtracking.git
cd Robotic_hand_handtracking
Set Up Python Environment:

bash
Kopiera
Redigera
pip install opencv-python pyserial
Upload Arduino Sketch:

Open controll.ino in the Arduino IDE.​

Connect your Arduino Uno R4 WiFi.​

Upload the sketch to the Arduino.​

Run Hand Tracking Script:

bash
Kopiera
Redigera
python hand.py
Ensure your webcam is connected and functioning properly.

Usage
Launch the Python script to start hand gesture recognition.​

The script processes video input from the webcam to detect hand positions.​
git.histb.com

Detected gestures are translated into commands sent to the Arduino via serial communication.​

The Arduino interprets these commands to control the servo motors, mimicking the hand gestures.​

File Structure
hand.py: Python script for capturing and processing webcam input to detect hand gestures.​

controll.ino: Arduino sketch for receiving commands and controlling servo motors accordingly.​

cad/: Directory containing CAD files for the 3D-printed robotic arm components.​

License
This project is open-source and available under the MIT License.​

For more details and updates, visit the GitHub repository.
