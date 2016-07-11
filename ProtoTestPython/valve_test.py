#!/usr/bin/python
import Adafruit_BBIO.GPIO as GPIO
import time

GPIO.setup("P8_8", GPIO.OUT)
GPIO.setup("P8_10", GPIO.IN)
GPIO.setup("P8_12", GPIO.IN)

if GPIO.input("P8_10"):
        print("Valve Closed")
else:
        print("Valve Open OR Moving")

GPIO.output("P8_8", GPIO.HIGH)

time.sleep(10)

if GPIO.input("P8_12"):
        print("Valve Open")
else:
        print("Valve Closed OR Moving")

GPIO.output("P8_8", GPIO.LOW)
GPIO.cleanup()

