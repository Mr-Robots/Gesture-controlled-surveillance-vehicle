import threading
import time
import serial
import io
import sys

tLock = threading.Lock()

def Tiva_Serial(port):
    ser=serial.Serial("/dev/tty"+ port,9600)

    if(ser.isOpen()==False):
        ser.open()
    a = 'a'
    st = ""
    while True:
        try:
            #sys.stdout.write(ser.read())
            #print ord(ser.read()),

            ser.flushInput()
            a = ser.read()
            st = ""
            while(a != chr(255)):
                a = ser.read()
                #print "Py Sync :", ord(a)
            for i in range (0,15):
                st += ser.read()
            sys.stdout.flush()
            sys.stdout.write(st)
            #time.sleep(0.06)

        except KeyboardInterrupt:
            ser.close()
            print "closed"
            break

def GPS_Tracker():
    ser=serial.Serial("/dev/ttyUSB0",9600)
    ser.open()
    Msg_ID=''
    Gps_str1=''
    Gps_str2=''
    temp1=''
    while True:
        temp1=''
        while Msg_ID!='GPGGA':
            while ser.read()!='$':
                continue
            Msg_ID=''
            for i in range(0,5):
                Msg_ID=Msg_ID+ser.read()
        Msg_ID=''
        for i in range(0,12):
            temp1=ser.read();
        for i in range(0,9):
            Gps_str1=Gps_str1+ser.read()

        temp=ser.read()
        temp=ser.read()
        temp1=ser.read()

        if temp!='N' and temp!='S':
            Gps_str1='pqrs'
            Gps_str2='abcd'
            #print Gps_str1
            #print Gps_str2
            continue
        else:
            for i in range(0,10):
                Gps_str2=Gps_str2+ser.read()
        print Gps_str1
        print Gps_str2
        Gps_str1=''
        Gps_str2=''



def Main():
    #t1=threading.Thread(target=GPS_Tracker)
    t2=threading.Thread(target=Tiva_Serial, args=("AMA0",))
    #t1.start()
    t2.start()
    t2.join()
    #t1.join()
    print "Main Complete"

if __name__== '__main__':
    Main()

