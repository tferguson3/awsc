#code to calculate flux of membranes
import csv
import datetime as dt
from datetime import date, timedelta, datetime
import os



yesterday = date.today() - timedelta(days=1)
year = yesterday.year
month = yesterday.month
day = yesterday.day
#for day in range(1,2):
print(day)
power1 = "flow"
power2 = "pressure"

# /home/addotson/data_AWSC/raw/

#filename1 = "/home/addotson/data_AWSC/raw/{0}_{1}_{2}_conex-{3}".format(year, month, day, power1)
#filename2 = "/home/addotson/data_AWSC/raw/{0}_{1}_{2}_conex-{3}".format(year, month, day, power2)

filename1 = "/users/EnvEngineering/flux/{0}_{1}_{2}_conex-{3}".format(year, month, day, power1)
filename2 = "/users/EnvEngineering/flux/{0}_{1}_{2}_conex-{3}".format(year, month, day, power2)
i=1
csv_reader1 = csv.reader(open(filename1+".csv"))
next(csv_reader1) #ignore headers
for line in csv_reader1:
    i=i+1
i=i+40
intpf=[0 for j in range(i)]
gwrf=[0 for j in range(i)]
intrf=[0 for j in range(i)]
wwpf=[0 for j in range(i)]
y=[0 for j in range(i)]
dayroflux=0
dayroflow=0
dayropress=0
daynfflux=0
daynfflow=0
daynfpress=0
j=0
csv_reader1 = csv.reader(open(filename1+".csv"))
next(csv_reader1)
for line in csv_reader1:
    #print(j)
    gwrf[j]=(float(line[1]))
    intrf[j]=(float(line[2]))
    intpf[j]=(float(line[3]))
    wwpf[j]=(float(line[4]))
    y[j]=(line[0])
    j=j+1

############## pressure reading
#csv_reader1 = csv.reader(open(filename2+".csv"))
i1=1
csv_reader1 = csv.reader(open(filename1+".csv"))
next(csv_reader1) #ignore headers
for line in csv_reader1:
    i1=i1+1
i1=i1+40
feed=[0 for j in range(i1)]
gwrp=[0 for j in range(i1)]
intrp=[0 for j in range(i1)]
fltr1=[0 for j in range(i1)]
fltr2=[0 for j in range(i1)]
y1=[0 for j in range(i1)]
j=0
csv_reader1 = csv.reader(open(filename2+".csv"))
next(csv_reader1)
for line in csv_reader1:
    #print(j)
    feed[j]=(float(line[1]))
    gwrp[j]=(float(line[2]))
    intrp[j]=(float(line[3]))
    fltr2[j]=(float(line[4]))
    fltr2[j]=(float(line[5]))
    y1[j]=(line[0])
    j=j+1
j=0
if y1[j]==y[j]:#make sure timestamps are equal
    err=0
    err1=0
else:
    if y1[j]==y[j+1]:
        err=1
        err1=0
    elif  y1[j]==y[j+2]:
            err=2
            err1=0
    elif  y1[j]==y[j+3]:
            err=3
            err1=0
    elif y1[j]==y[j+4]:
        err=4
        err1=0
    elif  y1[j]==y[j+5]:
            err=5
            err1=0
    elif  y1[j]==y[j+6]:
            err=6
            err1=0
    elif y1[j]==y[j+7]:
        err=7
        err1=0
    elif y1[j+1]==y[j]:
        err1=1
        err=0
    elif y1[j+2]==y[j]:
        err1=2
        err=0
    elif y1[j+3]==y[j]:
        err1=3
        err=0
    elif y1[j+4]==y[j]:
        err1=4
        err=0
    else:
        err1=0
        err=0
#print(err)
k=i1+30
ROcount=0
NFcount=0
filtervol=0
ROflux=[0 for j in range(k)]
NFflux=[0 for j in range(k)]
ROflow=[0 for j in range(k)]
NFflow=[0 for j in range(k)]
ROp=[0 for j in range(k)]
NFp=[0 for j in range(k)]
dayroflux=1
dayroflow=1
daynfflux=1
daynfflow=1
#print(y1)
#print(y)
for j in range(0,k):
    #print(y1[j+err1],y[j+err])
    if y1[j+err1]==0 and y[j+err]==0 and ROcount>10 and NFcount>10:
        break#print("end")
    elif y1[j+err1] != y[j+err]:
        if y1[j+err1]==y[j+err+1]:
            err1=err1-1
        elif  y1[j+err1]==y[j+err+2]:
                err=err+1
                err1=err1-1
        elif  y1[j+err1]==y[j+err+3]:
                err=err+2
                err1=err1-1
        elif y1[j+err1]==y[j+err+4]:
            err=err+3
            err1=err1-1
        elif y1[j+err1]==y[j+err-1]:
            err=err-1
        elif  y1[j+err1]==y[j+err-2]:
                err=err-2
        elif  y1[j+err1]==y[j+err-3]:
                err=err-3
        elif y1[j+err1]==y[j+err-4]:
            err=err-4
        else: j=j
    elif y1[j+err1] == y[j+err]:
        #print("noterror")
        if wwpf[j+err]>0 and intpf[j+err]==0:
            ROp[j]=(feed[j+err1]+intrp[j+err1])/2
            ROflux[j]=wwpf[j+err]/ROp[j]/0.0689476#equation for fluxgal/day/bar
            ROflow[j]=wwpf[j+err]/4
            ROcount=ROcount+1
        elif intpf[j+err]>0 and wwpf[j+err]==0:
            NFp[j]=(feed[j+err1]+gwrp[j+err1])/2
            NFflux[j]=intpf[j+err]/NFp[j]/0.0689476
            NFflow[j]=intpf[j+err]/4
            NFcount=NFcount+1
        if gwrf[j+err]!=0:#vol through prefilters
            filtervol=filtervol+gwrf[j+err]/4
#factors are for L/M2 from gal/ft2
dayroflux=sum(ROflux)/78/ROcount*1000#*3.79541/.0929
dayroflow=sum(ROflow)
dayropress=sum(ROp)/ROcount
daynfflux=sum(NFflux)/82/NFcount*1000#*3.79541/.0929
daynfflow=sum(NFflow)
daynfpress=sum(NFp)/NFcount

#print(NFcount)
#print"Total Prefilter Volume= %f gal"%(filtervol)
#print""
#print"NF flow= %f gal" %(daynfflow)
#print"NF ave.pressrue= %f psi"%(daynfpress)
print"NF Flux= %f gal/day/ft2/bar"%(daynfflux)
#print""
#print"RO flow= %f gal"%(dayroflow)
#print"RO ave.pressrue= %f psi"%(dayropress)
print"RO flux= %f gal/day/ft2/bar"%(dayroflux)


##writing to file next
filename="flowpress_analysis.csv"
file=open(filename,"a")
file.write("\n%i%i%i,%f,%f,%f,%f,%f,%f,%f" % (year,month,day,filtervol,daynfflow,daynfpress,daynfflux,dayroflow,dayropress,dayroflux))
file.close()
