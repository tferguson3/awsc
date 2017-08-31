/* Pins in Use:
     IR Sensor:       2
     PRessure sensor on A0
*/


int val;                        // variable for reading the serial input

#define BOUNCE_DURATION 200   // define an appropriate bounce time in ms for your switches

volatile unsigned long bounceTime=0; // variable to hold ms count to debounce a pressed switch
volatile long dropcount;
volatile long dropcount2;
volatile long strokerate;
 unsigned long drop_num;
 unsigned long lowflowtime;
 unsigned long highflowtime;
 float flowtarget;
 float flowrate;
 float flowratehigh;
 float flowratelow;
 unsigned long time_int;
 unsigned long time_set;
 unsigned long timeold;
 unsigned long totaldrops;
 unsigned long totaldropsold;
 float totalvol;
 float dropsize;
 unsigned long steppermax;
 unsigned long steppermin;
 const int feedppin=A0;
 int sfeedp = 0;
 int feedp = 0;


void setup() 
{                     
   Serial.begin(9600);
   attachInterrupt(0, dropcount_fun, RISING);  //pin 2 tr reciever
   pinMode(feedppin,INPUT);//pin A0
   drop_num = 2;
   dropcount = 3;
   dropcount2 = 0;
   strokerate = 118;
   lowflowtime = 0;
   highflowtime = 0;
   timeold = 0;
   totaldrops = 0;
   totaldropsold = 0;
   
   //set here!!
   flowtarget = 2.0; //mL per min target
   //set here!!
   
   time_set = flowtarget * 60 * 1000 * dropsize;
   totalvol = 0;
   
   //make sure this is correct!
   dropsize = 0.047;//how to find this
   //make sure this is correct!

   flowrate = 0;
   pressures();   
   pressures();   
   pressures();   
   pressures();   
   pressures();   
   pressures();
  Serial.print("Time");Serial.print("\t");
  Serial.print("PSI");Serial.print("\t");
  Serial.print("Drops");Serial.print("\t");
  Serial.print("Vol mL");Serial.print("\t");
  Serial.println("flowrate mL/min");
}  

 void loop()
 {      
   if (dropcount >= drop_num) { 
     //Update every 30 counts, increase this for better DPM resolution,
     //decrease for faster update
     time_int = ((millis() - timeold));
     timeold = millis();
     totaldrops = totaldropsold + dropcount;
     totaldropsold = totaldrops;
     totalvol = totaldrops * dropsize; //reports in mL
     flowrate = dropcount * dropsize / time_int * 1000 * 60; //report in mL per min
     flowratehigh = flowtarget * 1.03;
     flowratelow = flowtarget * 0.97;
     dropcount = 0;
     lowflowtime = time_set - (time_set / 10);
     highflowtime = time_set + (time_set / 10);
     pressures();
     pressures();
     pressures();
     pressures();
     pressures();
     //Serial.print("Time: ");
     Serial.print(timeold);Serial.print("\t");
     //Serial.print(" Pressure: ");
     Serial.print(sfeedp);Serial.print("\t");
     //Serial.print(" Drops: "); 
     Serial.print(totaldrops);Serial.print("\t");
     //Serial.print(" Volume (mL): ");
     Serial.print(totalvol);Serial.print("\t");
     //Serial.print(" FlowRate (mL/min): ");
     Serial.println(flowrate);}
      delay(1);
      if (Serial.available()){//type 0 for reset
        val = Serial.read();
      }
        if (val != 1) {          // if zero is sent by serial command it resets
            totalvol = 0;
            totaldropsold = 0; 
            Serial.println("RESET");   
        }
     val=1;                 // save the new state in our variable
         
   }  
 void dropcount_fun(){  
// this is the interrupt handler for button presses
// it ignores presses that occur in intervals less then the bounce time
  if(millis() > bounceTime)  
  {
      dropcount++;
      dropcount2++;
      bounceTime = millis() + BOUNCE_DURATION;  // set whatever bounce time in ms is appropriate
 }
}
void pressures(){//read and average pressure values
  int junk = analogRead(feedppin);
  feedp = (analogRead(feedppin)*.7018-54.74)*.868;
  sfeedp = (sfeedp+feedp)/2;
}
