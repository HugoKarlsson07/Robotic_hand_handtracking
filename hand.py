import cv2
import mediapipe as mp
import serial
import time

# Initialize serial communication with Arduino
try:
    arduino = serial.Serial(port='COM5', baudrate=9600, timeout=1)
    time.sleep(6)  # Wait for the serial connection to initialize
except serial.SerialException as e:
    print(f"Error: {e}")
    arduino = None

# Initialize Mediapipe
mp_hands = mp.solutions.hands
hands = mp_hands.Hands()
mp_drawing = mp.solutions.drawing_utils

#finktion that tracks the fingers in a index 0 for down and 1 for upp

def detect_fingers(hand_landmarks):
    finger_tips = [8, 12, 16, 20]  # Index, Middle, Ring, Pinky
    thumb_tip = 4
    finger_states = [0, 0, 0, 0, 0]  # Thumb, Index, Middle, Ring, Pinky

    # Check thumb
    if hand_landmarks.landmark[thumb_tip].x < hand_landmarks.landmark[thumb_tip - 1].x:
        finger_states[0] = 1  # Thumb is up

    # Check the other fingers
    for idx, tip in enumerate(finger_tips):
        if hand_landmarks.landmark[tip].y < hand_landmarks.landmark[tip - 2].y:
            finger_states[idx + 1] = 1  # Other fingers are up
    
    return finger_states

def linear_interpolation(value, in_min, in_max, out_min, out_max):
    return int(out_min + (float(value - in_min) / float(in_max - in_min)) * (out_max - out_min))

# Start capturing video
cap = cv2.VideoCapture(0) #chose the camera its a index that starts with 0

while cap.isOpened():
    success, image = cap.read()
    if not success:
        print("No hand present")
        break

    image = cv2.cvtColor(cv2.flip(image, 1), cv2.COLOR_BGR2RGB)
    results = hands.process(image)
    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

    if results.multi_hand_landmarks:
        for hand_landmarks in results.multi_hand_landmarks:
            mp_drawing.draw_landmarks(image, hand_landmarks, mp_hands.HAND_CONNECTIONS)
            fingers_state = detect_fingers(hand_landmarks)

            lmList = []
            for id, lm in enumerate(hand_landmarks.landmark):
                h, w, c = image.shape
                lm_x, lm_y = int(lm.x * w), int(lm.y * h)
                lmList.append([lm_x, lm_y])

            # Check if index finger tip is available
            if len(lmList) > 8:
                px, py = lmList[8]
                
                # Convert position to degree value for servo control
                ws, hs = image.shape[1], image.shape[0]
                servoX = linear_interpolation(px, 0, ws, 180, 0)
                servoY = linear_interpolation(py, 0, hs, 0, 180)

                # Display servo values on screen
                cv2.rectangle(image, (40, 20), (350, 110), (0, 255, 255), cv2.FILLED)
                cv2.putText(image, f'Servo X: {servoX} deg', (50, 50), cv2.FONT_HERSHEY_PLAIN, 2, (255, 0, 0), 2)
                cv2.putText(image, f'Servo Y: {servoY} deg', (50, 100), cv2.FONT_HERSHEY_PLAIN, 2, (255, 0, 0), 2)

                # Send data to Arduino in a single list
                #print("Hand detected, trying to send data...")#felsökning för att se om programmet kommer hit.
            if arduino:
                data = f"[{','.join(map(str, fingers_state))},{servoX},{servoY}]\n"
                print(f"Sending data: {data}")
                arduino.write(bytes(data, 'utf-8'))

                if arduino.in_waiting > 0:
                    response = arduino.readline().decode('utf-8').strip()
                    #print(f"Arduino response: {response}") #test svar från arduino


    cv2.imshow('Hand Tracking', image)
    if cv2.waitKey(5) & 0xFF == 27:  # Press 'ESC' to exit
        break

cap.release()
cv2.destroyAllWindows()
if arduino:
    arduino.close()
