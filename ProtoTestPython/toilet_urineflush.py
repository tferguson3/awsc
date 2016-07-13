#!/usr/bin/python
import Adafruit_BBIO.GPIO as GPIO
import time

GPIO.setup("P8_15", GPIO.OUT) #direction
GPIO.setup("P8_17", GPIO.OUT) #PWM

GPIO.output("P8_17", GPIO.HIGH)

GPIO.output("P8_15", GPIO.LOW)
time.sleep(3.1)

GPIO.output("P8_15", GPIO.HIGH)
time.sleep(4)

GPIO.cleanup()
