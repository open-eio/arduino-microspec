import glob, serial

port = glob.glob("/dev/ttyACM*")[0]

s = serial.Serial(port,baudrate=115200)

data = None
#readout several spec and only keep the last one
for i in range(10):
    s.write("SPEC.READ?\n")
    data = s.readline()
    print data

data = map(int,data.split(","))

from pylab import *
plot(data)
show()
