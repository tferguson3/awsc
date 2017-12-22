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
int x=0;
float starttime;
float tank;
float airtimer;
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
  lcd.clear();
  lcd.setCursor(0,1);
  measurevol();
  lcd.print(tank);
  measurevol();
  lcd.print(tank);
  measurevol();
  lcd.print(tank);
  measurevol();
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
  lcd.clear();
  lcd.setCursor(0,1);
  measurevol();
  lcd.print(tank);
  measurevol();
  lcd.print(tank);
  measurevol();
  lcd.print(tank);
  measurevol();
  lcd.print(tank);
  //write conditions for running sequence here and add functions
  while (x<1){
    normalsequence();
    x++;}
  }
void weekend(){
  aeration(15, 4);
  rest(45);
}
void normalsequence(){
  DateTime now = rtc.now();
  starttime = now.unixtime();
  //fill(10);
  now = rtc.now();
  starttime = now.unixtime();
  //fill(13);
  now = rtc.now();
  starttime = now.unixtime();
  fill(16);
  now = rtc.now();
  starttime = now.unixtime();
  fill(19);
  now = rtc.now();
  starttime = now.unixtime();
  fill(22);
  now = rtc.now();
  starttime = now.unixtime();
  fill(25);
  now = rtc.now();
  delay(5000);
  aeration(240, 24);
  delay(5000);
  settle(60, 24);
  delay(5000);
  skim(60);
  delay(5000);
  settle(120,24);
  delay(5000);
  decant(6);
  delay(5000);
  //rest(5);
  delay(5000);
}
void fill(int lvl){//check depth and fill until level is 25gal plus stir
  digitalWrite(stir, HIGH);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("FILL ");
  lcd.setBacklight(TEAL);
  measurevol();
  lcd.setCursor(0,1);
  lcd.print(tank);lcd.print("gal");
  DateTime now = rtc.now();
   while (now.unixtime()-starttime <3600000){
    
    DateTime now = rtc.now();
    while (tank<lvl){
      digitalWrite(fillpump, HIGH);
      measurevol();
      lcd.setCursor(0,1);
      lcd.print(tank);lcd.print("gal");
      delay(15000);
    }
    digitalWrite(fillpump, LOW);
    periodicaeration(15,60, now.unixtime());
    delay(15000);
}}

void periodicaeration(float ontime, float onfreq, float tt){//on for 15 min freq is 60 min
  ontime=ontime*60*1000;
  onfreq=onfreq*60*1000;
  if (onfreq-ontime>tt-starttime){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Periodic Aeration ");
  DateTime now = rtc.now();
  float current = now.unixtime();
  while (current-tt < ontime){
    float timedisplay= (ontime-(current-tt))*1000*60;
    lcd.setCursor(0,1);
  lcd.print(timedisplay);
  delay(1000);
  digitalWrite(air, HIGH);
  DateTime now = rtc.now();
  current = now.unixtime();
  }
digitalWrite(air, LOW);  }
  }
void aeration(float airduration, int lvl){//duration in min check depth and keep air on and pumps off for time period
  airduration=airduration*60*1000;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Aeration ");
  lcd.setBacklight(GREEN);
  measurevol();
  lcd.setCursor(0,1);
  lcd.print(tank);
  float starttime = millis();
  delay(100);
  while (((millis()-starttime)<airduration)&&(tank>lvl)){  //2 min
    digitalWrite(air, HIGH);
    measurevol();
    lcd.setCursor(0,1);
    lcd.print(tank);
    delay(20000);}
    digitalWrite(air, LOW);
}
void settle(float duration, int lvl){//everything off just timer
  duration=duration*60*1000;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Settle ");
  lcd.setBacklight(YELLOW);
  measurevol();
  lcd.setCursor(0,1);
  lcd.print(tank);
  float starttime = millis();
  delay(100);
  while (((millis()-starttime)<duration)&&(tank>lvl)){
    delay(10000);
    measurevol();
  lcd.setCursor(0,1);
  lcd.print(tank);} //2 min
}
void skim(float duration){//skim during settle
  duration=duration*60*1000;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Skim ");
  lcd.setBacklight(YELLOW);
  measurevol();
  lcd.setCursor(0,1);
  lcd.print(tank);
  float starttime = millis();
  delay(100);
  while ((millis()-starttime)<duration){//1min
    digitalWrite(skimmer, HIGH);
    lcd.setCursor(0,1);
  lcd.print(tank);
  delay(15000);}
    digitalWrite(skimmer, LOW);
}
void decant(int level){ // empty through decant pump checking volume
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Decant ");
  lcd.setBacklight(RED);
  measurevol();
  lcd.setCursor(0,1);
  lcd.print(tank);
  float starttime = millis();
 while (tank>level){
    digitalWrite(decantpump, HIGH);
    measurevol();
    lcd.setCursor(0,1);
  lcd.print(tank);
  delay(7000);}
    digitalWrite(decantpump, LOW);
}
void rest(float longt){ //everything off except maybe stir
  longt=longt*60*1000;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("REST ");
  lcd.setBacklight(VIOLET);
  measurevol();
  lcd.setCursor(0,1);
  lcd.print(tank);
  float starttime = millis();
  delay(100);
   while ((millis()-starttime)<longt){
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
  float tank1 = analogRead(depth)*0.421569-28.4559;
  float tank2 = analogRead(depth)*0.421569-28.4559;//*0.438596-32.7018;
  float tank3 = analogRead(depth)*0.421569-28.4559;//*0.438596-32.7018;
  float tank4 = analogRead(depth)*0.421569-28.4559;//*0.438596-32.7018;
  float tank5 = analogRead(depth)*0.421569-28.4559;//*0.438596-32.7018;
  float tank6 = analogRead(depth)*0.421569-28.4559;//*0.438596-32.7018;
  float tank7 = analogRead(depth)*0.421569-28.4559;//*0.438596-32.7018;
  float tank8 = analogRead(depth)*0.421569-28.4559;//*0.438596-32.7018;
  tank = (tank1 + tank2 + tank3 + tank4 + tank5 + tank6 + tank7 + tank8)/8;
}
