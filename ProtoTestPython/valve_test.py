#!/usr/bin/python
import Adafruit_BBIO.GPIO as GPIO
import time

valveswitch = 'P8_10'
closedlimit = 'P8_11'
openlimit = 'P8_14'

GPIO.setup(valveswitch, GPIO.OUT)
GPIO.setup(closedlimit, GPIO.IN)
GPIO.setup(openlimit, GPIO.IN)

if GPIO.input(closedlimit):
        print("Valve Closed")
else:
        print("Valve Open OR Moving")

GPIO.output(valveswitch, GPIO.HIGH)

time.sleep(10)

if GPIO.input(openlimit):
        print("Valve Open")
else:
        print("Valve Closed OR Moving")

GPIO.output(valveswitch, GPIO.LOW)
GPIO.cleanup()
