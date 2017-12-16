/*
  SD card datalogger

 This example shows how to log data from three analog sensors
 to an SD card using the SD library.

 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 10 

 */

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include <SoftwareSerial.h>      //we have to include the SoftwareSerial library, or else we can't use it. 

const int chipSelect = 10;

unsigned long LOG_INTERVAL = 60000;
unsigned long previousMillis = 0;


// The analog pins that connect to the sensors
#define PressureAnalog A1      // analog 1

// define the Real Time Clock object
RTC_PCF8523 RTC; 

char sensordata[30];                //we make a 48 byte character array to hold incoming data from the Flow Meter Totalizer.  
char sensordatajunk[30];
byte received_from_computer=0;     //we need to know how many characters have been received.                                 
byte sensor_bytes_received=0;       //we need to know how many characters have been received.
byte string_received=0;            //used to identify when we have received a string from the Flow Meter Totalizer.
float f;
char *flotot;

int PressureAnalogReading = 0;
unsigned long PressureReadingSUM = 0;
unsigned long readingCount = 0;

void setup() {
  // Open serial communications and wait for port to open:
 
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  // connect to RTC
  Wire.begin();  
}

void loop() {
  analogRead(PressureAnalog); 
  delay(10);

  delay(990); 
  
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= LOG_INTERVAL) {
    previousMillis = currentMillis;
    
    // create a filename
    
    DateTime now;
    now = RTC.now();
    int YR = now.year();
    String YRs = String(YR,DEC);
    int MO = now.month();
    String MOs = String(MO,DEC);
    int DY = now.day();
    //int DY = now.minute();  //testing clear command
    String DYs = String(DY,DEC);
    String DATE = YRs + MOs + DYs;
    String filename = DATE + ".CSV";
  
    //collect data
    //Average repetative current and voltage readings between sampling interval
    PressureAnalogReading = PressureReadingSUM / readingCount;    
    float P = PressureAnalogReading / 102.94;
    
    delay(10); 
    
    //read total flow once per sampling interval
    
    delay(200);
    
    // make a string for assembling the data to log:
    String dataString = "";
    dataString += String(now.unixtime());dataString += ",";
    dataString += String(P);
    
  
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open(filename, FILE_WRITE);
  
    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      // print to the serial port too:
      Serial.println(dataString);
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.print("error opening ");Serial.print(filename);Serial.println("!");
    }
  
    //reset averaging tools if interval is met
    readingCount=0; 
    PressureAnalogReading = 0;
  }
}
