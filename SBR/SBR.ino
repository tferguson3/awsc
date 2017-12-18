#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include "RTClib.h"
RTC_DS3231 rtc;
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7
int en1 = 2; //1 is fill
int step1 = 3;
int dir1 = 4;
int decantpump = 5;
int fillpump = 6;
int skimmer =7;
int air = 9;
int stir = 8;
int depth = A0;
float tank;
void setup() {
  pinMode(en1, OUTPUT);
  pinMode(decantpump, OUTPUT);
  pinMode(step1, OUTPUT);
  pinMode(fillpump, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(skimmer, OUTPUT);
  pinMode(air, OUTPUT);
  pinMode(stir, OUTPUT);
  pinMode(depth, INPUT);
  digitalWrite(en1, HIGH);
  lcd.begin(16, 2);
   if (! rtc.begin()) {
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
  // this library has been optimized a bit and we're proud of it :)
  lcd.print("start!");
  lcd.setBacklight(WHITE);
  delay(5000);
}
uint8_t i=0;
void loop() {
  // put your main code here, to run repeatedly:
  DateTime now = rtc.now();
  lcd.setCursor(0, 0);
  lcd.print(now.unixtime());
  lcd.setCursor(0, 1);
  lcd.print(millis()/1000);
delay(100);
  uint8_t buttons = lcd.readButtons();

  if (buttons) {
    lcd.clear();
    lcd.setCursor(0,0);
    if (buttons & BUTTON_UP) {
      lcd.print("UP ");
      lcd.setBacklight(RED);
    }
    if (buttons & BUTTON_DOWN) {
      lcd.print("DOWN ");
      lcd.setBacklight(YELLOW);
    }
    if (buttons & BUTTON_LEFT) {
      lcd.print("LEFT ");
      lcd.setBacklight(GREEN);
    }
    if (buttons & BUTTON_RIGHT) {
      lcd.print("RIGHT ");
      lcd.setBacklight(TEAL);
    }
    if (buttons & BUTTON_SELECT) {
      lcd.print("SELECT ");
      lcd.setBacklight(VIOLET);
    }
  }
  //write conditions for running sequence here and add functions
  fill();
  delay(5000);
  aeration();
  delay(5000);
  settle();
  delay(5000);
  skim();
  delay(5000);
  settle();
  delay(5000);
  decant();
  delay(5000);
  rest();
  delay(5000);
}
void fill(){//check depth and fill until level is 25gal plus stir
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("FILL ");
  lcd.setBacklight(TEAL);
  measurevol();
  lcd.setCursor(0,1);
  lcd.print(tank);
  int starttime = millis();
   while (tank<15){
    digitalWrite(fillpump, HIGH);
    measurevol();
    delay(20000);}
    digitalWrite(fillpump, LOW);
}
void aeration(){//check depth and keep air on and pumps off for time period
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Aeration ");
  lcd.setBacklight(GREEN);
  measurevol();
  lcd.setCursor(0,1);
  lcd.print(tank);
  int starttime = millis();
  while (((millis()-starttime)<120000)&&(tank>23)){  //2 min
    digitalWrite(air, HIGH);
    measurevol();
    delay(20000);}
    digitalWrite(air, LOW);
}
void settle(){//everything off just timer
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Settle ");
  lcd.setBacklight(YELLOW);
  measurevol();
  lcd.setCursor(0,1);
  lcd.print(tank);
  int starttime = millis();
  while (((millis()-starttime)<120000)&&(tank>23)){ } //2 min
}
void skim(){//skim during settle
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Skim ");
  lcd.setBacklight(YELLOW);
  measurevol();
  lcd.setCursor(0,1);
  lcd.print(tank);
  int starttime = millis();
  while ((millis()-starttime)<60000){//1min
    digitalWrite(skimmer, HIGH);}
    digitalWrite(skimmer, LOW);
}
void decant(){ // empty through decant pump checking volume
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Decant ");
  lcd.setBacklight(RED);
  measurevol();
  lcd.setCursor(0,1);
  lcd.print(tank);
  int starttime = millis();
 while (tank>7){
    digitalWrite(decantpump, HIGH);
    measurevol();}
    digitalWrite(decantpump, LOW);
}
void rest(){ //everything off except maybe stir
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("REST ");
  lcd.setBacklight(VIOLET);
  measurevol();
  lcd.setCursor(0,1);
  lcd.print(tank);
  int starttime = millis();
   while ((millis()-starttime)<10000){
    digitalWrite(stir, HIGH);}
    digitalWrite(stir, LOW);
}
void stepper(int num, int motor, int direct){
  if (motor == 1){
    digitalWrite(en1, LOW); 
    if (direct == 1){
      digitalWrite(dir1, HIGH);}//empty
      else if (direct ==2){
        digitalWrite(dir1, LOW);}// fill
    for (int i=0; i<num; i++){
      digitalWrite(step1, HIGH);
      delay(3);
      digitalWrite(step1, LOW);
      delay(3);
  }
  digitalWrite(en1, HIGH);
}
}

void measurevol(){
  int junk = analogRead(depth);
  float tank1 = analogRead(depth)*0.438596-30.7018;
  float tank2 = analogRead(depth)*0.438596-30.7018;
  float tank3 = analogRead(depth)*0.438596-30.7018;
  float tank4 = analogRead(depth)*0.438596-30.7018;
  float tank5 = analogRead(depth)*0.438596-30.7018;
  float tank6 = analogRead(depth)*0.438596-30.7018;
  float tank7 = analogRead(depth)*0.438596-30.7018;
  float tank8 = analogRead(depth)*0.438596-30.7018;
  tank = (tank1 + tank2 + tank3 + tank4 + tank5 + tank6 + tank7 + tank8)/8;
}