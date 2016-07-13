#!/usr/bin/python
import Adafruit_BBIO.GPIO as GPIO
import time, sys

GPIO.setup("P8_26", GPIO.IN)

global count
count = 0

def countPulse(channel):
   global count
   count = count+1
   print count

GPIO.add_event_detect("P8_26", GPIO.RISING, callback=countPulse)

while True:
    try:
        time.sleep(1)
    except KeyboardInterrupt:
        print '\ncaught keyboard interrupt!, bye'
        GPIO.cleanup()
        sys.exit()
