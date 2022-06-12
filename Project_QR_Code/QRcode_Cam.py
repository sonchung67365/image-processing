import cv2 as cv
import numpy as np
from pyzbar.pyzbar import decode
import serial
import time

arduino = serial.Serial(port='COM3', baudrate=9600)
time.sleep(2)
print("Established serial connection to Arduino")

#mo file text
with open('mDataFile.text') as f:
    myDataList = f.read().splitlines()
    print(myDataList)

cap = cv.VideoCapture(0)
if not cap.isOpened():
    print("Can't not open camera!")
    exit()
while True:
    #ghi hinh frame
    ret, frame = cap.read()
    if not ret:
        print("Can't receive frame (stream end?). Exiting ...")
        break
    #lat hinh
    frame = cv.flip(frame, 1)
    #doc ma QR
    val = str(0)
    for barcode in decode(frame):
        #print(barcode.rect)
        myData = barcode.data.decode('utf-8')
        #print(myData)
        if myData in myDataList:
            mText = "OK"
            mColor = (0, 255, 0)
            print(mText)
            val = str(1)
        else:
            mText = "NO!"
            mColor = (0, 0, 255)
            print(mText)
            val = str(0)
        #hien duong bao QR
        pts = np.array([barcode.polygon], np.int32)
        pts = pts.reshape((-1, 1, 2))
        cv.polylines(frame, [pts], True, mColor, 5)
        #hien noi dung QR
        pts2 = barcode.rect
        px = pts2[0]
        py = pts2[1]
        cv.putText(frame, mText, (px, py), cv.FONT_HERSHEY_SIMPLEX, 1, mColor, 2)
        #cv.putText(frame, myData, (pts2[0], pts2[1]), cv.FONT_HERSHEY_SIMPLEX, 0.9, (0, 255, 0), 2)
    arduino.write(bytes(val, 'utf-8'))
    #hien thi webcam
    cv.imshow("Webcam", frame)
    if cv.waitKey(1) == ord('q'):
        break

cap.release()
cv.destroyAllWindows()
