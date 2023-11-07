import cv2
import serial
import numpy as np

# Configure the serial port (replace 'COMx' with the appropriate port on your system)
ser = serial.Serial('COMx', 115200)

while True:
    frame_bytes = b''
    while True:
        frame_bytes += ser.read(1024)
        if b'\xff\xd8' in frame_bytes and b'\xff\xd9' in frame_bytes:
            frame = cv2.imdecode(np.frombuffer(frame_bytes, dtype=np.uint8), cv2.IMREAD_COLOR)
            cv2.imshow('ESP32-CAM Stream', frame)
            frame_bytes = b''

        if cv2.waitKey(1) & 0xFF == ord('x'):
            break

ser.close()
cv2.destroyAllWindows()
