#!/usr/local/bin/python3

import serial, io
import time


device   = '/dev/cu.usbmodemFA131' # serial port
baud     = 9600                          # baud rate
filename = 'bald-log.txt'                # log file to save data in

with serial.Serial(device,baud) as serialPort, open(filename,'wb') as outFile:
        while(1):
                line = serialPort.readline() # must send \n! get a line of log
                timestamp = int(time.time())
                print(timestamp), "        ",(line)
                #print("/t")
                #print (line)                 # show line on screen
                #outFile.write(timestamp)
                #outFile.write(/t)
                outFile.write("%s\t%s" % (timestamp,line))          # write lin$
                outFile.flush()              # make sure it actually gets writt$



