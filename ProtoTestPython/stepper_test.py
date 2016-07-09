#!/usr/bin/python
import Adafruit_BBIO.GPIO as GPIO
import time

GPIO.setup("P8_9", GPIO.OUT) #step pin
GPIO.setup("P8_11", GPIO.OUT) #direction pin

##Define a function named Blink()
def Blink(numTimes,speed):
    for i in range(0,numTimes):## Run loop numTimes
        GPIO.output("P8_11", GPIO.HIGH)
        print "Iteration " + str(i+1)## Print current loop
        GPIO.output("P8_9", GPIO.HIGH)
        time.sleep(speed)## Wait
        GPIO.output("P8_9", GPIO.LOW)
        time.sleep(speed)## Wait
    for i in range(0,numTimes):## Run loop numTimes
        GPIO.output("P8_11", GPIO.LOW)
        print "Iteration " + str(i+1)## Print current loop
        GPIO.output("P8_9", GPIO.HIGH)
        time.sleep(speed)## Wait
        GPIO.output("P8_9", GPIO.LOW)
        time.sleep(speed)## Wait
    print "Done" ## When loop is complete, print "Done"
    GPIO.cleanup()

## Ask user for total number of blinks and length of each blink
iterations = raw_input("Enter total number of times to step: ")
speed = raw_input("Enter length of each step sequence (seconds): ")

## Start Blink() function. Convert user input from strings to numeric data types and pass to Blink() as parameters
Blink(int(iterations),float(speed))
