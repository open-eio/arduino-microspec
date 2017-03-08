import glob, json
import serial

port = glob.glob("/dev/ttyACM*")[-1]

ser = serial.Serial(port,baudrate=115200)

from pylab import *
from matplotlib import pyplot as plt
fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)
plt.ion()

fig.canvas.draw()

l1, = ax1.plot(arange(288),[0]*288)

ax1.set_xlim(0,288)
ax1.set_ylim(0,2**12)
plt.show()


while True:
    try:
        line = ser.readline()
        data = json.loads(line.decode('utf8'))
        spec = array(data)
        print(spec)
        l1.set_xdata(arange(288))
        l1.set_ydata(spec)
        #ax1.draw_artist(l1)
        fig.canvas.draw()
        fig.canvas.flush_events()
    except ValueError:
        pass
