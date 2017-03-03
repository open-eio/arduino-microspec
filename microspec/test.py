import os, glob, serial
from pylab import *

class MicroSpec(object):
    def __init__(self, port):
        self._ser = serial.Serial(port,baudrate=115200)
    def set_integration_time(self, seconds):
        self._ser.write(b"SPEC.INTEG %0.6f\n")
    def read(self):
        self._ser.write(b"SPEC.READ?\n")
        data = self._ser.readline()
        data = array([int(p) for p in data.split(b",")])
        return data

if __name__ == "__main__":
    DATASET_NAME = "CFL_16bit_integ100us"
    mpl.rcParams["savefig.directory"] = os.path.dirname(__file__)

    port = glob.glob("/dev/ttyACM*")[0]
    
    spec = MicroSpec(port)
    spec.set_integration_time(100e-6)
    
    data = spec.read()
    
    plot(data)
    title(DATASET_NAME)
    show()
    savetxt("%s.csv" % DATASET_NAME,data,delimiter=",")
