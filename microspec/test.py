import os, glob, serial
from pylab import *


DATASET_NAME = "fluor_overhead_10bit"

mpl.rcParams["savefig.directory"] = os.path.dirname(__file__)


port = glob.glob("/dev/ttyACM*")[0]

s = serial.Serial(port,baudrate=115200)

def read_spec():
    data = None
    #the first spec will usually be saturated
    for i in range(2):
        s.write(b"SPEC.READ?\n")
        data = s.readline()
        
    data = array([int(p) for p in data.split(b",")])
    return data


data = read_spec()
plot(data)
title(DATASET_NAME)
show()
savetxt("%s.csv" % DATASET_NAME,data,delimiter=",")
