#!/usr/bin/python
import Adafruit_BBIO.GPIO as GPIO
import Adafruit_BBIO.PWM as PWM
import time, sys

#step pin on motor driver to be controlled by PWM below *note changed to P9_16 as not all listed PWM pins appear to function
GPIO.setup("P8_7", GPIO.OUT) #direction pin
GPIO.setup("P8_11", GPIO.OUT) #ENABLE motor driver
GPIO.setup("P9_41", GPIO.IN) #flowmeter

GPIO.output("P8_11", GPIO.HIGH)
GPIO.output("P8_7", GPIO.LOW) #direction

global count
global totalcount

count = 0
totalflow = 0
PWMstarted = 0

def countPulse(channel):
   global count
   count = count+1

GPIO.add_event_detect("P9_41", GPIO.RISING, callback=countPulse)

while True:
    try:
        start_counter = 1
        count=0
        time.sleep(1)
        start_counter = 0
        flow = count * 60.0 / 2200.0
	if flow >= 4.1: #set maximum flow to restrict miscalculation associated with electrical noise acceptable due to pd pump
		flow = 4.1
        stepf = flow * 0.02 * 1000 / 60 / 2.36 * 200
        totalflow = totalflow + flow/60
        print '%s%f\t%s%f\t%s%f' % ( "Flow (LPM): ",flow,"Step rate (Hz): ",stepf,"Total Flow (L):",totalflow)

        if stepf >= 5 and PWMstarted == 0:
            GPIO.output("P8_11", GPIO.LOW)
            PWM.start("P9_16", 50, stepf, 1)
            PWMstarted = 1

        elif stepf >= 5 and PWMstarted == 1:
            PWM.set_frequency("P9_16", stepf)

        elif stepf < 5 and PWMstarted == 1:
            PWM.stop("P9_16")
            PWM.cleanup()
            GPIO.output("P8_11", GPIO.HIGH)
            PWMstarted = 0

        #else:
            #stepf < 5 and PWMstarted == 0 is do nothing
    except KeyboardInterrupt:
        print '\ncaught keyboard interrupt!, bye'
        GPIO.cleanup()
        sys.exit()
