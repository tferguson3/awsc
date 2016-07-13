//Senior Design
//Greg Schmidt, Lance Leber, Brittany Boring?
//02/03/2016

/*
 * DESCRIPTION:
 *   This program is to control the power distribution to 4 water heaters. There is a priority given to the
 *   water heaters depending on the definition of flowMeterPins and relayControlPins below. This priority 
 *   comes into effect when multiple WHs are supplying water. When no water is flowing it just cycles through
 *   for up to MAXRUNTIME milliseconds on each WH. Each water heater has a thermostat which independently
 *   controls the temperature of the heater. To determine if the thermostat has the WH off we are using a
 *   current coil around the main power supply to the system. 
 *   Hall-effect flow meters are using to indicate when water is flowing.
 *   The arduino uno rev3 supports up to 6 water heaters without addition hardware. This code will support
 *   up to 8. This code can be used with a different arduino board. 
 * 
 * OUTPUT: 
 *   DIGITAL PIN 3, 4, 5, 6.   Control the relays. 
 *
 * INPUT: 
 *   DIGITAL PIN 8, 9, 10, 11.   Input from flow meter. 
 *   ANALOG PIN A4 Is the pin used for the main power current coil signal.
 *
 * SETTINGS:
 *   WHCOUNT = the number of waters heaters in the system
 *   MIN_FLOW_RATE = Threshold for flow meters in pulses per second. Lower == more sensitive to flow
 *   CURRENTTHRESH = Threshold for main current meter. Measured in volts. 1 ~= 4.9mV
 *   MINSWITCHTIME = Minimum time between switching water heaters in milliseconds
 *   MAXRUNTIME = Maximum amount of time for a water heater to run before switching, when no water is flowing.
 * VARIABLES:
 *   flowMeterPins[] = The pins the flow meter signal is connected. Leftmost is highest priority.
 *   currentXfmrPin = Analog pin the current coils signal is connected.
 *   relayControlPins = The digital output pins for controlling the relays. Leftmost is highest priority
 *   waterRunIndication = bit addressed varaible to indicate which WH has flowing water. (supports up to 8 WHs)
 *   waterHeatRun = bit addressed variable to inidcate which WH relay is ON. (supports up to 8 WHs)
 *   intCount = integer used to determine flow rate of Flow meters.
 *   lastSwitch = used to indicate the time it last switch a water heater in milliseconds.
 *   lastWaterRun = used to inidcate when the water in a WH stopped flowing. This allows the heater to run for up to
 *                  the MAXRUNTIME after the water stops flowing, in milliseconds.
 *   WaterRun = bool used to determine if water flow was recently started/stopped.
 * 
 * FUNCTIONS:
 *   void setup() = sets the relayControlPins as an output pin and the flowMeterPins as input pins.
 *   void interruptCount() = helper function to determine flow rate. Increments a global variable intCount
 *   void ReadFlowMeters() = Loops through all flowMeterPins. On each interation it sets a interrupt on the respective pin.
 *                           Each time that pin sees a rising edge it will call interruptCount(). After .5 seconds the interrupt
 *                           is removed. If the count is greater than MIN_FLOW_RATE the corresponding BIT is set in waterRunIndication.
 *                           This function requires the PinChangeInt.h library.
 *   bool currentFlow() = returns true if voltage level on the currentXfmrPin(value * 4.9mV) if it exceeds CURRENTTHRESH.
 *                        returns false otherwise.
 *   int currentHeaterOn() = helper function that returns the index of the water heater that is currently recieving power. 
 *   void switchHeater(int toHeater) = function that controls switching from one water heater to another. If toHeater is
 *                                     -1 the function will loop to the next water heater in the rotation. Otherwise will
 *                                     switch to the indicated heater. 
 *   void loop() = function that contains the main logic to control the water heaters. This function is called in a loop
 *                 by the arduino.
 *
 */

#include <PinChangeInt.h>
#define WHCOUNT 4
#define MIN_FLOW_RATE 100 // In Hertz;
#define CURRENTTHRESH 660 // About 3.5 volts
#define MINSWITCHTIME 5 * 1000 // 5 Seconds
#define MAXRUNTIME  (15*60*1000) 

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
 


//Priority goes from index 0 to 3. 
int flowMeterPins[WHCOUNT] = { 7, 8, 9, 10 }; // Signal from the hall-effect flow meters
int currentXfmrPin = A4; // Signal from the current transformer
int relayControlPins[WHCOUNT] = { 3, 4, 5, 6 }; // Pins to control relays

//Priority goes from Least significant bit to most. 
byte waterRunIndication = 0; // Bit addressed, idicates if water was run. 
byte waterHeatRun = 0; // Bit addressed, indicates which control pin is high.

int intCount; // Varible used in interrupt to calculate Flow rate.

 

unsigned long lastSwitch;
unsigned long lastWaterRun;

bool WaterRun;


void setup() {
  // Set the input and output of each device. \
  Serial.begin(9600);
  for(int cnt = 0; cnt < WHCOUNT; cnt++){
	  pinMode( relayControlPins[cnt], OUTPUT); // Defines all relay control pins to be outputs.
	  pinMode( flowMeterPins[cnt], INPUT);
  }

  WaterRun = false;
  lastSwitch = 0;
  lastWaterRun = 0;
  switchWaterHeater(-1);
  Serial.println("Setup complete, beginning");
  
}
// Interrupt Service Routing (ISR)
void interruptCount(){
	intCount++;
}



void ReadFlowMeters(){
        int val; // digital IO pin storage bin
	waterRunIndication = 0; // Set all water run indication to 0.
//  for(int cnt = 0; cnt < WHCOUNT; cnt++)
//  {
//	  intCount = 0;
//	  PCintPort::attachInterrupt(flowMeterPins[cnt], interruptCount, RISING);
//	  delay(500);
//	  PCintPort::detachInterrupt(flowMeterPins[cnt]);

          /*Serial.print("WH ");
          Serial.print(cnt);
          Serial.print("is flowing ");
	  double flowRate = (intCount * 2) * (.007229); // gals/min (X P/sec) * (1/8300 Gal/P) * 60 sec/ min = gal/min
          Serial.println(flowRate);*/
//	  if((intCount * 2) > MIN_FLOW_RATE)
//	  {
//                  Serial.print("WH ");
//                 Serial.print(cnt);
//                  Serial.print("is flowing ");
//                  Serial.println(intCount*2);
//		  bitSet(waterRunIndication, cnt); // Set bit cnt to be a 1 to indicate water is flowing. 
		  //We can break this for loop because the highest priority water heater will have the least significant bit
//		  break;
//	  }

//  }
     for (int cnt = 0 ; cnt < WHCOUNT ; cnt++){
     
          val = digitalRead(flowMeterPins[cnt]);
          if (val == 0)
              {
                Serial.print("WH ");
                Serial.print(cnt);
                Serial.print(" is flowing ");
                Serial.println(" ");
                bitSet(waterRunIndication, cnt); // Set bit cnt to be a 1 to indicate water is flowing. 
                break;  
                //We can break this for loop because the highest priority water heater will have the least significant bit
              }
      }
}


//Returns true if current is flowing above CURRENTTHRESH
//Reads the value ever 1 ms for 32 seconds, or 2 cycles;
bool currentFlow(){
	int maxc = 0;
	int temp =0;
	for(int i = 0; i < 32; i++){
		temp = analogRead(currentXfmrPin);
		if(temp > maxc){
			maxc = temp;
}
		delay(1);
	}
	if(maxc > CURRENTTHRESH)
		return true;
         Serial.println("Water heater is not on");
	return false;
}

void loop() {
  
	if((unsigned long)(millis() - lastSwitch) >= MINSWITCHTIME)
	{
		ReadFlowMeters(); //Update which water heaters are running, if any.
		if(waterRunIndication == 0){
			if(WaterRun == true){										//If water was running last cycle and is no longer.
				lastWaterRun = millis();								//Set the time in which the water was last running
				WaterRun = false;
			}

			if((unsigned long)(millis() - lastWaterRun) >= MAXRUNTIME){ //If running longer than our max run time, switch.
				switchWaterHeater(-1);									//Changes the water heater that is running.
			}
			else if(currentFlow() == false){								//If no current is flow (heater is off), switch.
				switchWaterHeater(-1);									//Changes the water heater that is running.
			}
		}
		else{
			if(WaterRun == false)										//Make sure WaterRun is indicated as true.
				WaterRun = true;										

			if(waterRunIndication != waterHeatRun){
				for(int cnt = 0; cnt < WHCOUNT; cnt++){
					if(bitRead(waterRunIndication, cnt) == 1){			//Turn on the first water heater that indicated flow.
						switchWaterHeater(cnt);
						break;											//break the loop so only the highest priority heater is on.
					}
				}
			}
		}
            lastSwitch = millis();
	}
	delay(500);														//delay or sleep some amount of milliseconds
}

//return the index of the water heater that is currently on. 
int currentHeaterOn(){
	for(int cnt = 0; cnt < WHCOUNT; cnt++){
		if(bitRead(waterHeatRun, cnt) == 1){							//Turn on the first water heater that indicated flow.
			return cnt;													//break the loop so only the highest priority heater is on.
		}
	}
	return 0;
}
 
void switchWaterHeater(int toHeater){
      int stoHeater = toHeater;//Temp variable 
	if(stoHeater == -1){													//If -1 we loop through normally. We can add a fancy algorithm if we would like.
		int curHeater = currentHeaterOn();								//Grab the current water heater that is on.
		stoHeater = curHeater + 1;											//Increment to the next one
		if(stoHeater >= WHCOUNT)											//Check for wrapping around to 0
			stoHeater = 0;
	}
        Serial.print("Switching to relay: ");
        Serial.println(stoHeater);
	for(int cnt = 0; cnt < WHCOUNT; cnt++){
		digitalWrite(relayControlPins[cnt], LOW);						//Turn off all water heaters
	}
	waterHeatRun = 0;													//Reset heater on indication
	digitalWrite(relayControlPins[stoHeater], HIGH);						//Turn on indicate water heater
	bitSet(waterHeatRun, stoHeater);										//Set heater on indication

}

