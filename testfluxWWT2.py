#code to calculate flux of membranes
import csv
import datetime as dt
from datetime import date, timedelta, datetime
import os



yesterday = date.today() - timedelta(days=1)
year = yesterday.year
month = yesterday.month
day = yesterday.day
#for day in range(1,7):
#print(day)
power1 = "iFlow"
power2 = "Pressure"

## /home/addotson/data_AWSC/raw/

filename1 = "/home/addotson/data_AWSC/raw/{0}_{1}_{2}_WWT-{3}".format(year, month, day)
filename2 = "/home/addotson/data_AWSC/raw/{0}_{1}_{2}_WWT-{3}".format(year, month, day)

#filename1 = "/users/EnvEngineering/flux/{0}_{1}_{2}_conex-{3}".format(year, m$
#filename2 = "/users/EnvEngineering/flux/{0}_{1}_{2}_conex-{3}".format(year, m$
i=1
csv_reader1 = csv.reader(open(filename1+".txt"),delimeter="\t")
next(csv_reader1) #ignore headers
for line in csv_reader1:
    i=i+1
i=i+40
NFpf=[0 for j in range(i)]
NFrf=[0 for j in range(i)]
ROrf=[0 for j in range(i)]
ROpf=[0 for j in range(i)]
Cpf=[0 for j in range(i)]
y=[0 for j in range(i)]
ddayroflux=0
dayroflow=0
dayropress=0
daynfflux=0
daynfflow=0
daynfpress=0
pf1=0
pf2=0
pf3=0
j=0
csv_reader1 = csv.reader(open(filename1+".txt"),delimeter="\t")
next(csv_reader1)
for line in csv_reader1:
    #print(j)
    NFrf[j]=(float(line[2]))
    ROrf[j]=(float(line[4]))
    NFpf[j]=(float(line[1]))
    ROpf[j]=(float(line[3]))
    Cpf[j]=(float(line[0]))
    y[j]=(line[5])
    j=j+1
############## pressure reading
#csv_reader1 = csv.reader(open(filename2+".csv"))
############## pressure reading
#csv_reader1 = csv.reader(open(filename2+".csv"))
i1=1
csv_reader1 = csv.reader(open(filename2+".txt"),delimeter="\t")
next(csv_reader1) #ignore headers
for line in csv_reader1:
    i1=i1+1
i1=i1+40
feed=[0 for j in range(i1)]
NFrp=[0 for j in range(i1)]
ROrp=[0 for j in range(i1)]
fltr1=[0 for j in range(i1)]
fltr2=[0 for j in range(i1)]
y1=[0 for j in range(i1)]
j=0
csv_reader1 = csv.reader(open(filename2+".txt"),delimeter="\t")
next(csv_reader1)
for line in csv_reader1:
    #print(j)
    feed[j]=(float(line[0]))
    NFrp[j]=(float(line[3]))
    ROrp[j]=(float(line[4]))
    fltr1[j]=(float(line[1]))
    fltr2[j]=(float(line[2]))
    y1[j]=(line[5])
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
pfpcount=0
filtervol=0
ROflux=[0 for j in range(k)]
NFflux=[0 for j in range(k)]
ROflow=[0 for j in range(k)]
NFflow=[0 for j in range(k)]
ROp=[0 for j in range(k)]
NFp=[0 for j in range(k)]
pfp=[0 for j in range(k)]
pfp1=[0 for j in range(k)]
pfp2=[0 for j in range(k)]
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
        if ROpf[j+err]>0.1 and NFpf[j+err]<0.1:
            ROp[j]=(feed[j+err1]+ROrp[j+err1])/2
            ROflux[j]=ROpf[j+err]/ROp[j]/0.0689476#equation for fluxgal/day/bar
            ROflow[j]=ROpf[j+err]*(y1[j+err]-y1[j+err-1])/60
            ROcount=ROcount+1
        elif NFpf[j+err]>0.1 and ROpf[j+err]<0.1:
            NFp[j]=(feed[j+err1]+NFrp[j+err1])/2
            NFflux[j]=NFpf[j+err]/NFp[j]/0.0689476
            NFflow[j]=NFpf[j+err]*(y1[j+err]-y1[j+err-1])/60
            NFcount=NFcount+1
        if Cpf[j+err]!=0:#vol through prefilters
            filtervol=filtervol+Cpf[j+err]*(y1[j+err]-y1[j+err-1])/60
            if NFpf[j+err]<0.1 and ROpf[j+err]<0.1:
                pfp[j]=feed[j+err1]
                pfp1[j]=fltr1[j+err1]
                pfp2[j]=fltr2[j+err1]
                pfpcount=pfpcount+1
#factors are for L/M2 from gal/ft2
dayroflux=sum(ROflux)/78/ROcount*1000#*3.79541/.0929
dayroflow=sum(ROflow)
dayropress=sum(ROp)/ROcount
daynfflux=sum(NFflux)/82/NFcount*1000#*3.79541/.0929
daynfflow=sum(NFflow)
daynfpress=sum(NFp)/NFcount
pf1=sum(pfp)/pfpcount
pf2=sum(pfp1)/pfpcount
pf3=sum(pfp2)/pfpcount
#print(NFcount)
#print"Total Prefilter Volume= %f gal"%(filtervol)
#print""
#print"NF flow= %f gal" %(daynfflow)
#print"NF ave.pressrue= %f psi"%(daynfpress)
#print"NF Flux= %f gal/day/ft2/bar"%(daynfflux)
#print""
#print"RO flow= %f gal"%(dayroflow)
#print"RO ave.pressrue= %f psi"%(dayropress)
#print"RO flux= %f gal/day/ft2/bar"%(dayroflux)


##writing to file next
filename="flowpress_analysis.csv"
file=open(filename,"a")
file.write("%i_%i_%i,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n" % (year,month,day,filtervol,daynfflow,daynfpress,daynfflux,dayroflow,dayropress,dayroflux,pf1,pf2,pf3))
file.close()
