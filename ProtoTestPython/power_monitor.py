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
# "AIN0", "P9_39"
# "AIN1", "P9_40"
# "AIN2", "P9_37"
# "AIN3", "P9_38"
# "AIN4", "P9_33"
# "AIN5", "P9_36"
# "AIN6", "P9_35"

sensor_pin1 = 'P9_40'
sensor_pin2 = 'P9_37'
sensor_pin3 = 'P9_38'
sensor_pin4 = 'P9_33'
sensor_pin5 = 'P9_36'
sensor_pin6 = 'P9_35'


now=time.localtime(time.time())
currentmonth=now.tm_mon
currentday=now.tm_mday
currentyear=now.tm_year
filename = "{0}_{1}_{2}_cabin-power.txt".format(currentyear, currentmonth, currentday)

#### informative messaging for starting storage file
print "Opening ",filename, " for appending..."
print "reading analog inputs and storing data..."
file=open(filename,"a")
file.write("Time,Reading,Volts,Amps\n")
file.close()
#initialize averaging counter
AVE_reading1=0
AVE_volts1=0
AVE_AMPS1=0

AVE_reading2=0
AVE_volts2=0
AVE_AMPS2=0

AVE_reading3=0
AVE_volts3=0
AVE_AMPS3=0

AVE_reading4=0
AVE_volts4=0
AVE_AMPS4=0

AVE_reading5=0
AVE_volts5=0
AVE_AMPS5=0

AVE_reading6=0
AVE_volts6=0
AVE_AMPS6=0

count=0


#### Begin measurement loop
while True:
        #read analog pin #
        reading1 = ADC.read(sensor_pin1)  
        reading2 = ADC.read(sensor_pin2)  
        reading3 = ADC.read(sensor_pin3)  
        reading4 = ADC.read(sensor_pin4)  
        reading5 = ADC.read(sensor_pin5)  
        reading6 = ADC.read(sensor_pin6)  

        # increment counter for average calculation
        count = count+1

        #get current time
        now=time.localtime(time.time())
        pt=time.asctime(now)  #formatted time for file
        currentmonth=now.tm_mon
        currentday=now.tm_mday
        currentyear=now.tm_year


        #scale reading back to voltage
        volts1=reading1*1.800  
        volts2=reading2*1.800
        volts3=reading3*1.800
        volts4=reading4*1.800
        volts5=reading5*1.800
        volts6=reading6*1.800
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
        volts1 = volts1 / 0.36  #correct for 1000ohm, 562ohm voltage divider
        volts2 = volts2 / 0.36  
        volts3 = volts3 / 0.36  
        volts4 = volts4 / 0.36  
        volts5 = volts5 / 0.36  
        volts6 = volts6 / 0.36  

        # No jumper -- CT scaled for 0-10A
        ### uncomment if setting used
        AMPS1=volts1/0.5 #Volts / (V/A) = amps
        AMPS2=volts2/0.5 #Volts / (V/A) = amps
        AMPS3=volts3/0.5 #Volts / (V/A) = amps
        AMPS4=volts4/0.5 #Volts / (V/A) = amps
        AMPS5=volts5/0.5 #Volts / (V/A) = amps
        AMPS6=volts6/0.5 #Volts / (V/A) = amps
        # mid jumper -- CT scaled for mid 0-20 amps
        #uncomment if setting used#
        #AMPS = volts / 0.25

        # high jumper -- CT output scaled for high, 0-50 amps
        #uncomment if setting used#
        #AMPS = volts / 0.1

        #write to the screen #
        ###comment this out when not testing####
        #print pt,'\t%s\t%f\t%f\t%f\t%i' % ( "1",reading1,volts1,AMPS1,count)
        #print pt,'\t%s\t%f\t%f\t%f\t%i' % ( "2",reading2,volts2,AMPS2,count)
        #print pt,'\t%s\t%f\t%f\t%f\t%i' % ( "3",reading3,volts3,AMPS3,count)
        #print pt,'\t%s\t%f\t%f\t%f\t%i' % ( "4",reading4,volts4,AMPS4,count)
        #print pt,'\t%s\t%f\t%f\t%f\t%i' % ( "5",reading5,volts5,AMPS5,count)
        #print pt,'\t%s\t%f\t%f\t%f\t%i' % ( "6",reading6,volts6,AMPS6,count)

        if (now.tm_sec == 0)&(count>10): #if new minute, write data to file
                                         # and write only one value for second $
                AVE_reading1=(AVE_reading1+reading1)/count
                AVE_volts1=(AVE_volts1+volts1)/count
                AVE_AMPS1=(AVE_AMPS1+AMPS1)/count
                print pt,'AVE-\t%s\t%f\t%f\t%f' % ( "1",AVE_reading1,AVE_volts1,AVE_AMPS1)

                AVE_reading2=(AVE_reading2+reading2)/count
                AVE_volts2=(AVE_volts2+volts2)/count
                AVE_AMPS2=(AVE_AMPS2+AMPS2)/count
                print pt,'AVE-\t%s\t%f\t%f\t%f' % ( "2",AVE_reading2,AVE_volts2,AVE_AMPS2)

                AVE_reading3=(AVE_reading3+reading3)/count
                AVE_volts3=(AVE_volts3+volts3)/count
                AVE_AMPS3=(AVE_AMPS3+AMPS3)/count
                print pt,'AVE-\t%s\t%f\t%f\t%f' % ( "3",AVE_reading3,AVE_volts3,AVE_AMPS3)

                AVE_reading4=(AVE_reading4+reading4)/count
                AVE_volts4=(AVE_volts4+volts4)/count
                AVE_AMPS4=(AVE_AMPS4+AMPS4)/count
                print pt,'AVE-\t%s\t%f\t%f\t%f' % ( "4",AVE_reading4,AVE_volts4,AVE_AMPS4)

                AVE_reading5=(AVE_reading5+reading5)/count
                AVE_volts5=(AVE_volts5+volts5)/count
                AVE_AMPS5=(AVE_AMPS5+AMPS5)/count
                print pt,'AVE-\t%s\t%f\t%f\t%f' % ( "5",AVE_reading5,AVE_volts5,AVE_AMPS5)

                AVE_reading6=(AVE_reading6+reading6)/count
                AVE_volts6=(AVE_volts6+volts6)/count
                AVE_AMPS6=(AVE_AMPS6+AMPS6)/count
                print pt,'AVE-\t%s\t%f\t%f\t%f' % ( "6",AVE_reading6,AVE_volts6,AVE_AMPS6)


                #open file to append
                file=open(filename,"a")
                #add first column date/time stamp
                file.write(pt)
                #add next columns with raw reading, and converted voltage
                file.write(",%s,%f,%f,%f\n" % ("1",AVE_reading1,AVE_volts1,AVE_AMPS1))
                #add first column date/time stamp
                file.write(pt)
                file.write(",%s,%f,%f,%f\n" % ("2",AVE_reading2,AVE_volts2,AVE_AMPS2))
                #add first column date/time stamp
                file.write(pt)
                file.write(",%s,%f,%f,%f\n" % ("3",AVE_reading3,AVE_volts3,AVE_AMPS3))
                #add first column date/time stamp
                file.write(pt)
                file.write(",%s,%f,%f,%f\n" % ("4",AVE_reading4,AVE_volts4,AVE_AMPS4))
                #add first column date/time stamp
                file.write(pt)
                file.write(",%s,%f,%f,%f\n" % ("5",AVE_reading5,AVE_volts5,AVE_AMPS5))
                #add first column date/time stamp
                file.write(pt)
                file.write(",%s,%f,%f,%f\n" % ("6",AVE_reading6,AVE_volts6,AVE_AMPS6))
                #close file, in case of program termination prematurely,
                file.close()
                #if MM/DD/YR changes, update filename
                #this translates to a new file every day
                ##!!!!header row is dropped from subsequent days
                filename = "{0}_{1}_{2}_cabin-power.txt".format(currentyear, currentmonth, currentday)
                #reset averageing counters
                AVE_reading1=0
                AVE_volts1=0
                AVE_AMPS1=0

                AVE_reading2=0
                AVE_volts2=0
                AVE_AMPS2=0

                AVE_reading3=0
                AVE_volts3=0
                AVE_AMPS3=0

                AVE_reading4=0
                AVE_volts4=0
                AVE_AMPS4=0

                AVE_reading5=0
                AVE_volts5=0
                AVE_AMPS5=0

                AVE_reading6=0
                AVE_volts6=0
                AVE_AMPS6=0

                count=0
        else:   # continue summing readings for average
                AVE_reading1=AVE_reading1+reading1
                AVE_volts1=AVE_volts1+volts1
                AVE_AMPS1=AVE_AMPS1+AMPS1

                AVE_reading2=AVE_reading2+reading2
                AVE_volts2=AVE_volts2+volts2
                AVE_AMPS2=AVE_AMPS2+AMPS2

                AVE_reading3=AVE_reading3+reading3
                AVE_volts3=AVE_volts3+volts3
                AVE_AMPS3=AVE_AMPS3+AMPS3

                AVE_reading4=AVE_reading4+reading4
                AVE_volts4=AVE_volts4+volts4
                AVE_AMPS4=AVE_AMPS4+AMPS4

                AVE_reading5=AVE_reading5+reading5
                AVE_volts5=AVE_volts5+volts5
                AVE_AMPS5=AVE_AMPS5+AMPS5

                AVE_reading6=AVE_reading6+reading6
                AVE_volts6=AVE_volts6+volts6
                AVE_AMPS6=AVE_AMPS6+AMPS6



        #sleep for (.1 s)
        time.sleep(.1)

        ####currently, program only stops with a CTRL-C
