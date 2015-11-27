import serial

ser = serial.Serial('/dev/ttyUSB2',38400)
while True:
    try:
        x = ser.read()
        f=open('gesture_command.txt','w')
        f.write(x)
        f.close()
    except:
        print "Gesture serial : port error!"
        break
