#!/usr/bin/python
import Adafruit_BBIO.GPIO as GPIO
import time, sys

GPIO.setup("P9_12", GPIO.IN)
GPIO.setup("P9_15", GPIO.IN)
GPIO.setup("P9_23", GPIO.IN)

global count1
global count2
global count3

count1 = 0
count2 = 0
count3 = 0

def countPulse1(channel):
   global count1
   count1 = count1+1

def countPulse2(channel):
   global count2
   count2 = count2+1

def countPulse3(channel):
  global count3
  count3 = count3+1

GPIO.add_event_detect("P9_12", GPIO.RISING, callback=countPulse1)
GPIO.add_event_detect("P9_15", GPIO.RISING, callback=countPulse2)
GPIO.add_event_detect("P9_23", GPIO.RISING, callback=countPulse3)

while True:
    start_counter = 1
    count1=0
    count2=0
    count3=0
    time.sleep(1)
    start_counter = 0
    flow1 = (count1 * 0.3256 + 5.2004)
    flow2 = (count2 * 0.3256 + 5.2004)
    flow3 = (count3 * 0.3256 + 5.2004)
    print '\t%s\t%i\t%s\t%i\t%s\t%i' % ( "1: ",flow1,"2: ",flow2,"3: ",flow3)
    time.sleep(1)
    count1=0
    count2=0
    count3=0
    try:
        time.sleep(1)
    except KeyboardInterrupt:
        print '\ncaught keyboard interrupt!, bye'
        GPIO.cleanup()
        sys.exit()
