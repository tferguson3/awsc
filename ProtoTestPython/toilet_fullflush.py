#!/usr/bin/python
import Adafruit_BBIO.GPIO as GPIO
import time

GPIO.setup("P8_16", GPIO.OUT) #direction
GPIO.setup("P8_18", GPIO.OUT) #PWM

GPIO.output("P8_18", GPIO.HIGH)

GPIO.output("P8_16", GPIO.LOW)
time.sleep(2.25)

GPIO.output("P8_16", GPIO.HIGH)
time.sleep(4)

GPIO.cleanup()
