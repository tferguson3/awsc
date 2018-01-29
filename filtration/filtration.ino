int analogPin = A5;
int switchPin = 10;
float pressure = 0;
float pressure1 = 0;

//A variable to track pressure.
int interval = 15000;
float P = 0;
long sec_old = 0; 
long sec = 0;

//When this threshold is met, turn off power to pump.
int Pressure = 100;
int flag =0; //1 if pressure was exceded
void setup() {
  pinMode(switchPin,OUTPUT);
  // put your setup code here, to run once:
  //Initiate Serial communication.
  Serial.begin(9600);
  Serial.print("Time"); 
  Serial.print("\t");
  Serial.println("Pressure");
  digitalWrite(switchPin, LOW);
  
}//close setup

void loop() {
  sec_old = sec;
  delay(interval);
  sec = interval/1000 + sec_old;
  Serial.print(sec); 
  Serial.print("\t");
  pressure= (analogRead(analogPin)*.7018-48.42)*.868;
  pressures();
  pressures();
  pressures();
  pressures();
  pressures();
  Serial.println(pressure);
  if(pressure <= 75 && flag == 0){
    digitalWrite(switchPin, HIGH);
  }else{
    flag =1;
    digitalWrite(switchPin, LOW);
  }
}

void pressures(){//read and average pressure values
  int junk = analogRead(analogPin);
    P = analogRead(analogPin);
  pressure1 = (P*.7018-48.42)*.868;
  pressure = (pressure+pressure1)/2;
}
