
unsigned long t=0; 
unsigned long tpump=0;
unsigned long tair=0;
int airstatus=0;
int pumpstatus=0;
//pins
const int Pump = 8;
const int Air = 7;
//Edit these times below to change factors
unsigned long PumpInterval= 60000 *360;//How often in min
unsigned long AirInterval= 60000 *15;//How often in min
unsigned long PumpRunTime = 60000 *.5;//How Long in min
unsigned long AirRunTime = 60000 *1;//How Long in min

void setup() {
pinMode(Pump, OUTPUT);
pinMode(Air, OUTPUT);
//convert to milli sec
/*PumpInterval = 60000 * PumpInterval;
AirInterval = 60000 * AirInterval;
PumpRunTime = 60000 * PumpRunTime;
AirRunTime = 60000 * AirRunTime;*/
tair = AirInterval;
tpump = PumpInterval;
}
void AirFunction(int s){
  if (s==1 && pumpstatus==0){
    digitalWrite(Air, HIGH);
    airstatus = 1;
    tair =t;
  }
  else if (s ==0){
    digitalWrite(Air, LOW);
    airstatus =0;
  }
}
void PumpFunction(int s){
  if (s==1){
    digitalWrite(Pump, HIGH);
    pumpstatus = 1;
    tpump =t;
  }
  else if (s ==0){
    digitalWrite(Pump, LOW);
    pumpstatus =0;
}}
void waiting(){//function to read and report everything at given intervals
  t= millis() +PumpInterval;
  if (t-tair<0){}
  else if ((t-tair>0) && t-tair > AirInterval + AirRunTime){
    AirFunction(1);
  }
  else if (t-tair > AirRunTime && airstatus==1){
    AirFunction(0);
  }
  if (t-tpump<0){}
  else if ((t-tpump>0) && (t-tpump > PumpInterval+PumpRunTime)){
    AirFunction(0);
    PumpFunction(1);
  }
  else if (t-tpump>0 && t-tpump > PumpRunTime){
    PumpFunction(0);
  }
}
void loop() {
  delay(5000);
  waiting();
}
