/* Pins in Use:
     IR Sensor:       2
     Motor Driver:    8, 9, 10, 11
     LCD:             12, 13, 6, 5, 4, 3 
     Button:          7 
*/

//#include <LiquidCrystal.h>
//#include <Stepper.h> 

//const int stepsPerRevolution = 200;
//Stepper myStepper(stepsPerRevolution, 8,9,10,11); 

//int switchPin = 7;
int val;                        // variable for reading the serial input
//int buttonState;                // variable to hold the button state

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
//LiquidCrystal lcd(12, 13, 6, 5, 4, 3);  // initialize the library with the numbers of the interface pins




void setup() 
{                     
   Serial.begin(9600);
   attachInterrupt(0, dropcount_fun, RISING);  //pin 2 tr reciever
   pinMode(feedppin,INPUT);//pin A0
   //pinMode(switchPin, INPUT);    // sets the digital pin 7 as input to read buttonswitch
   drop_num = 5;
   dropcount = 0;
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
   dropsize = 0.023;//how to find this
   //make sure this is correct!

   flowrate = 0;
   //steppermax = 120;
   //steppermin = 0;
   
  /*/ set up the LCD's number of columns and rows: 
  lcd.begin(20, 4);
  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print("Flow:");
  lcd.setCursor(0, 1);
  lcd.print("Volume:");
  lcd.setCursor(0, 2);
  lcd.print("Auto:");
  lcd.setCursor(0, 3);
  lcd.print("Manual:");    */ 
}  

 void loop()
 { /*
      do
      {
      //lcd.setCursor(17, 0);
      //lcd.print(dropcount);
      } while (dropcount <= drop_num);*/
     
   if (dropcount >= drop_num) { 
     //Update every 30 counts, increase this for better DPM resolution,
     //decrease for faster update
     //lcd.setCursor(17, 0);
     //lcd.print("   ");   
     //lcd.setCursor(17, 1);
     //lcd.print("   ");
     //lcd.setCursor(17, 1);
     //lcd.print(strokerate);   
     time_int = ((millis() - timeold));
     timeold = millis();
     totaldrops = totaldropsold + dropcount;
     totaldropsold = totaldrops;
     totalvol = totaldrops * dropsize; //reports in L
     flowrate = dropcount * dropsize / time_int * 1000 * 60; //report in mL per min
     flowratehigh = flowtarget * 1.03;
     flowratelow = flowtarget * 0.97;
     dropcount = 0;
     lowflowtime = time_set - (time_set / 10);
     highflowtime = time_set + (time_set / 10);
     //lcd.setCursor(7, 0);
     //lcd.print(flowrate);
     pressures();
     pressures();
     pressures();
     pressures();
     pressures();
     Serial.print("Time: ");Serial.print(timeold);
     Serial.print(" Pressure: ");Serial.print(sfeedp);
     Serial.print(" Drops: "); Serial.print(totaldrops);
     Serial.print(" Volume (L): ");Serial.print(totalvol);
     Serial.print(" FlowRate (mL/min): ");Serial.println(flowrate);}
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
  
     //lcd.setCursor(7, 1);
     //lcd.print(totalvol);
        
   } 
    
    /*if (dropcount2 = 30 && strokerate <= steppermax && strokerate >= steppermin && flowrate <= flowratehigh && flowrate >= flowratelow) {
      lcd.setCursor(7, 2);
      lcd.print("No Change   ");   
      lcd.setCursor(7, 3);
      lcd.print("None    ");
      dropcount2 = 0;
    } else if (dropcount2 = 30 && flowrate < flowratelow && strokerate <= steppermax && strokerate >= steppermin) {
      lcd.setCursor(7, 2);
      lcd.print("Rate Inc + ");
      lcd.setCursor(7, 3);
      lcd.print("None    ");  
      myStepper.step(1);
      strokerate++;
      strokerate++;
      dropcount2 = 0; 
    } else if (dropcount2 = 30 && flowrate > flowratehigh && strokerate <= steppermax && strokerate >= steppermin) { 
      lcd.setCursor(7, 2);
      lcd.print("Rate Dec - ");
      lcd.setCursor(7, 3);
      lcd.print("None    ");
      myStepper.step(-1);
      strokerate--;
      strokerate--;
      dropcount2 = 0; 
   } else if (strokerate = steppermax) {
     lcd.setCursor(7, 2);
     lcd.print("MAN REQ!  ");
     lcd.setCursor(7, 3);
     lcd.print("Stroke -");
     dropcount2 = 0; 
   } else if (strokerate = steppermin) {
     lcd.setCursor(7, 2);
     lcd.print("MAN REQ!  ");
     lcd.setCursor(7, 3);
     lcd.print("Stroke +");
     dropcount2 = 0; 
  }*/
  
 
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
  feedp = analogRead(feedppin)*.21-16.5;
  sfeedp = (sfeedp+feedp)/2;
}
