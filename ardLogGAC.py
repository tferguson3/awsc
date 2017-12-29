#!/usr/local/bin/python3

import serial, io
import time


device   = '/dev/cu.usbmodem1431' # serial port
baud     = 9600                          # baud rate
now=time.localtime(time.time())
currentyear=now.tm_year
currentmonth=now.tm_mon
currentday=now.tm_mday
filename = '{0}_{1}_{2}GAC-log.txt'.format(currentyear,currentmonth,currentday)                # log file to save data in

with serial.Serial(device,baud) as serialPort, open(filename,'wb') as outFile:
	while(1):
		line = serialPort.readline() # must send \n! get a line of log
		timestamp = int(time.time())
		print(timestamp), "        ",(line)
		#print("/t")
		#print (line)                 # show line on screen
		#outFile.write(timestamp)
		#outFile.write(/t)
		outFile.write("%s\t%s" % (timestamp,line))          # write line of text to file
		outFile.flush()              # make sure it actually gets written out
