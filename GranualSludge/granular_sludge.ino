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
int air = 2; 
int fill = 3;
int decant = 4;
//int slowsludge = 5;

int x=0;
float TimeToFill=800000;//14.52min
float airtimer=24000000;//400min
float thirtysecairtime = 1800000;//30min 
float decanttime=60000;//1min
unsigned long timenow;

void setup() {
  pinMode(air, OUTPUT);
  pinMode(decant, OUTPUT);
  pinMode(fill, OUTPUT);
 // pinMode(slowsludge, OUTPUT);
  digitalWrite(air, LOW);
  digitalWrite(fill, LOW);
  digitalWrite(decant, LOW);
  //digitalWrite(slowsludge, LOW);
  lcd.begin(16, 2);
  lcd.print("start!");
  lcd.setBacklight(WHITE);
  delay(5000);
  lcd.clear();
  lcd.setCursor(0,1);
}
void fillup(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("FILL ");
  lcd.setBacklight(TEAL);
  unsigned long startfill = millis();
  x=0;
  digitalWrite(fill,HIGH);
  while (x==0){
    timenow=millis();
      if ((timenow-startfill) > TimeToFill){//5min
       digitalWrite(fill,LOW);
       x=1;
      }
      else if (timenow-startfill < TimeToFill){
       digitalWrite(fill,HIGH);
       int timedisplay = round((TimeToFill-(timenow-startfill))/1000);
       lcd.setCursor(0,1);
       lcd.print(timedisplay);lcd.print("sec  ");
      }
      delay(200);
    }
  digitalWrite(air, LOW);
  digitalWrite(fill, LOW);
  digitalWrite(decant, LOW);
    
}
void aeration(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Aeration ");
  lcd.setBacklight(GREEN);
  unsigned long startair = millis();
  x=0;
  digitalWrite(air,HIGH);
  while (x==0){
      if (millis()-startair > airtimer){//5min
       digitalWrite(air,LOW);
       x=1;
      }
      else if (millis()-startair < airtimer){
       digitalWrite(air,HIGH);
       int timedisplay = round((airtimer-(millis()-startair))/60/1000);
       lcd.setCursor(0,1);
       lcd.print(timedisplay);lcd.print("min  ");
      }
      delay(200);
    }
  digitalWrite(air, LOW);
  digitalWrite(fill, LOW);
  digitalWrite(decant, LOW);
      
}
void thirtysecaeration(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("30Aeration ");
  lcd.setBacklight(GREEN);
  unsigned long starttime = millis();
  delay(100);
  while (x==0){
      if ((millis()-starttime)>thirtysecairtime){//5min
       digitalWrite(air,LOW);
       x=1;
      }
      else if ((millis()-starttime)<thirtysecairtime){
       digitalWrite(air,HIGH);
       int timedisplay = round((thirtysecairtime-(millis()-starttime))/60/1000);
       lcd.setCursor(0,1);
       lcd.print(timedisplay);lcd.print("min  ");
       digitalWrite(air, HIGH);
    delay(30000);
    digitalWrite(air, LOW);
    delay(30000);
    delay(30000);
    delay(30000);
    delay(30000);
      }
      delay(200);
    }
  digitalWrite(air, LOW);
  digitalWrite(fill, LOW);
  digitalWrite(decant, LOW);
}
void rest(float resttime){//rest time in ms
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("REST ");
  lcd.setBacklight(VIOLET);
  unsigned long startrest = millis();
  digitalWrite(air, LOW);
  digitalWrite(fill, LOW);
  digitalWrite(decant, LOW);
  
  x=0;
  while (x==0){
      if (millis()-startrest > resttime){
       x=1;
      }
      else if (millis()-startrest < resttime){
       int timedisplay = round((resttime-(millis()-startrest))/60/1000);
       lcd.setCursor(0,1);
       lcd.print(timedisplay);lcd.print("min  ");
      }
      delay(20);
    }
}
void settlerest(float resttime){//rest time in ms
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("settle EST ");
  lcd.setBacklight(VIOLET);
  unsigned long startrest = millis();
  digitalWrite(air, LOW);
  digitalWrite(fill, LOW);
  digitalWrite(decant, LOW);
  
  x=0;
  while (x==0){
      if (millis()-startrest > resttime){
       x=1;
      }
      else if (millis()-startrest < resttime){
       int timedisplay = round((resttime-(millis()-startrest))/1000);
       lcd.setCursor(0,1);
       lcd.print(timedisplay);lcd.print("sec  ");
      }
      delay(20);
    }
}
void decantdrain(){//rest time in ms
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("DECANT ");
  lcd.setBacklight(RED);
  unsigned long startdecant = millis();
  digitalWrite(decant, HIGH);
  x=0;
  while (x==0){
      if (millis()-startdecant > decanttime){
       x=1;
       digitalWrite(decant, LOW);
      }
      else if (millis()-startdecant < decanttime){
       digitalWrite(decant, HIGH);
       int timedisplay = round((decanttime-(millis()-startdecant))/1000);
       lcd.setCursor(0,1);
       lcd.print(timedisplay);lcd.print("sec  ");
      }
      delay(20);
    }
  digitalWrite(air, LOW);
  digitalWrite(fill, LOW);
  digitalWrite(decant, LOW);
  delay(7000);
}

void normalsequence(){//~8hr cycle
  delay(2000);
  fillup();//14.5min
  delay(2000);
  thirtysecaeration();//1800000 = 30 min
  delay(2000);
  aeration();//400min
  delay(2000);
  settlerest(195000);//3min 15secs --time to settle before decant
  delay(2000);
  decantdrain();//1min
  delay(2000);
  rest(2160000);//50min - 14 for the fill
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  delay(100);
  lcd.setCursor(0,1);
  //decantdrain();
  while (x<1){
   //fillup();
   //aeration();
   //delay(1000);
   //decant(6);//loading();
    x++;}
    //normalsequence();
}
