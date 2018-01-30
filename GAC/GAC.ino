#include <SPI.h>
#include <Wire.h>
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
int flag =0; //1 if pressure was exceded
unsigned long LOG_INTERVAL = 15000;
unsigned long previousMillis = 0;
#define switchPin A2
#define PressureAnalog A0      // analog 1
float PressureAnalogReading = 0;
float PressureReadingSUM = 0;
unsigned long readingCount = 0;

void setup() {
  pinMode(switchPin,OUTPUT);
    digitalWrite(switchPin, HIGH);
    lcd.begin(16, 2);
  lcd.print("start!");
  lcd.setBacklight(WHITE);
  delay(5000);

uint8_t i=0;
  // Open serial communications and wait for port to open:
 
  Serial.begin(9600);
}

void loop() {//add shutoff? will require wiring of switch to arduino
  float t=millis();
  float number= (analogRead(PressureAnalog)*.34405-25);
  PressureReadingSUM = PressureReadingSUM + number;
  int runtime = t/1000/60;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GAC ");
  lcd.print(runtime);lcd.print("min   ");
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
    
    // 
    //collect data
    //Average repetative current and voltage readings between sampling interval
    PressureAnalogReading = PressureReadingSUM / readingCount;    
    float P = PressureAnalogReading;
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(P);
    delay(10000); 
    Serial.println(P);
    //read total flow once per sampling interval
    delay(1000); 
    //reset averaging tools if interval is met
    readingCount=0; 
    PressureReadingSUM = 0;
    if(P <= 20 && flag==0){
    digitalWrite(switchPin, HIGH);
    }
    else if (P>20){ flag ==1;
      digitalWrite(switchPin, LOW);}
    while (flag ==1){
      digitalWrite(switchPin, LOW); 
   }
  }
}
