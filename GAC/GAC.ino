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
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7
const int chipSelect = 10;

unsigned long LOG_INTERVAL = 25000;
unsigned long previousMillis = 0;

// The analog pins that connect to the sensors
#define PressureAnalog A0      // analog 1

// define the Real Time Clock object
RTC_PCF8523 RTC; 

char sensordata[30];                //we make a 48 byte character array to hold incoming data from the Flow Meter Totalizer.  
char sensordatajunk[30];
byte received_from_computer=0;     //we need to know how many characters have been received.                                 
byte sensor_bytes_received=0;       //we need to know how many characters have been received.
byte string_received=0;            //used to identify when we have received a string from the Flow Meter Totalizer.
float f;
char *flotot;

float PressureAnalogReading = 0;
float PressureReadingSUM = 0;
unsigned long readingCount = 0;

void setup() {
    lcd.begin(16, 2);
  /* if (! rtc.begin()) {
    lcd.print("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    lcd.print("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  // Print a message to the LCD. We track how long it takes since
  // this library has been optimized a bit and we're proud of it :)*/
  lcd.print("start!");
  lcd.setBacklight(WHITE);
  delay(5000);

uint8_t i=0;
  // Open serial communications and wait for port to open:
 
  Serial.begin(9600);
  /*while (!Serial) {
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
  Wire.begin();  */
}

void loop() {
  float t=millis();
  float number= (analogRead(PressureAnalog)*.7018-52)*.868;
  PressureReadingSUM = PressureReadingSUM + number;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GAC ");
  lcd.setCursor(0,1);
  lcd.print(number);//lcd.print(PressureAnalogReading);
  lcd.setCursor(6,1);
  lcd.print(t);
  lcd.setBacklight(TEAL);
  delay(10);
  readingCount=readingCount+1;
  delay(990); 
  
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= LOG_INTERVAL) {
    previousMillis = currentMillis;
    
    // create a filename
    /*
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
  */
    //collect data
    //Average repetative current and voltage readings between sampling interval
    PressureAnalogReading = PressureReadingSUM / readingCount;    
    float P = PressureAnalogReading;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(P);
    delay(10000); 
    Serial.println(P);
    //read total flow once per sampling interval
    
    delay(200);
    
    /*/ make a string for assembling the data to log:
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
      lcd.setCursor(0,1);
    lcd.print("it works");
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.print("error opening ");Serial.print(filename);Serial.println("!");
    }
  */
    delay(10000); 
    //reset averaging tools if interval is met
    readingCount=0; 
    PressureReadingSUM = 0;
  }
}
