import Adafruit_BBIO.ADC as ADC
import time

## not using ntplib for network time protocol or a real time clock,
## i am using processor time for the time calls.
## the clock seems to reset whenever the BBBlack is powered down
## thus the date-time needs to be reset prior to running the CT_Monitor
##>> sudo date -s 'YYYY-M-D 11:11:11'
##              '2016-5-27 11:50:00' as an example

####setup ADC and assign input pin
ADC.setup()
#analog input pins are
# "AIN4", "P9_33"
# "AIN6", "P9_35"
# "AIN5", "P9_36"
# "AIN2", "P9_37"
# "AIN3", "P9_38"
# "AIN0", "P9_39"
# "AIN1", "P9_40"

sensor_pin = 'P9_40'

now=time.localtime(time.time())
currentmonth=now.tm_mon
currentday=now.tm_mday
currentyear=now.tm_year
filename = "{0}_{1}_{2}_CT.txt".format(currentmonth, currentday, currentyear)

#### informative messaging for starting storage file
print "Opening ",filename, " for appending..."
print "reading analog inputs and storing data..."
file=open(filename,"a")
file.write("Time,Reading,Volts,Amps\n")
file.close()
#initialize averaging counter
AVE_reading=0
AVE_volts=0
AVE_AMPS=0
count=0


#### Begin measurement loop
while True:
        #read analog pin #
        reading = ADC.read(sensor_pin)

        # increment counter for average calculation
        count = count+1

        #get current time
        now=time.localtime(time.time())
        pt=time.asctime(now)  #formatted time for file
        currentmonth=now.tm_mon
        currentday=now.tm_mday
        currentyear=now.tm_year
 

        #scale reading back to voltage
        volts=reading*1.800
        #scale 1.8V reading back up to output 5V for CT
        ###############################
        ##  CT
        ## +o-------/
        ##  |       \ R1=a*R
        ##  5V      /
        ##  |       \
        ##  |      ------o AIpin
        ##  |    R2=\      BBB
        ##  |     R /      1.8V
        ##  |       \
        ## -o-------/-----o GND
        ##
        ##      a=16/9 a<=0.36 to keep from overvoltage at BBB
        ##      R <=  1kOhm
        ##      or use trim pot and adjust until
        ##       Vout = 0.36V for 1 Vin
        #############################
        volts = volts / 0.36

        # No jumper -- CT scaled for 0-10A
        ### uncomment if setting used
        AMPS=volts/0.5 #Volts / (V/A) = amps

        # mid jumper -- CT scaled for mid 0-20 amps
        #uncomment if setting used#

        # high jumper -- CT output scaled for high, 0-50 amps
        #uncomment if setting used#
        #AMPS = volts / 0.1

        #write to the screen #
        ###comment this out when not testing####
        print pt,'\t%f\t%f\t%f\t%i' % ( reading,volts,AMPS,count)

        if (now.tm_sec == 0)&(count>10): #if new minute, write data to file
                                         # and write only one value for second $
                AVE_reading=(AVE_reading+reading)/count
                AVE_volts=(AVE_volts+volts)/count
                AVE_AMPS=(AVE_AMPS+AMPS)/count
                print pt,'AVE-\t%f\t%f\t%f' % ( AVE_reading,AVE_volts,AVE_AMPS)

                #open file to append
                file=open(filename,"a")
                #add first column date/time stamp
                file.write(pt)
                #add next columns with raw reading, and converted voltage
                file.write(",%f,%f,%f\n" % (AVE_reading,AVE_volts,AVE_AMPS))
                #close file, in case of program termination prematurely,
                file.close()
                #if MM/DD/YR changes, update filename
                #this translates to a new file every day
                filename = "{0}_{1}_{2}_CT.txt".format(currentmonth, currentday, currentyear)
                #reset averageing counters
                AVE_reading=0
                AVE_volts=0
                AVE_AMPS=0
                count=0
        else:   # continue summing readings for average
                AVE_reading=AVE_reading+reading
                AVE_volts=AVE_volts+volts
                AVE_AMPS=AVE_AMPS+AMPS



        #sleep for (.1 s)
        time.sleep(.1)

        ####currently, program only stops with a CTRL-C
