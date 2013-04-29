import numpy as np
import streamplot
import time

# create a stream plot
spo2_plot = streamplot.StreamPlot(saveFileNameStart = "trial_spo2",lineColors = ['r','b'])

# first, a ramp-up
for i in range(100):
	time.sleep(0.05)
	spo2_plot.addDataPoint(i, [i,i+2])

# then, the signal stays constant
for i in range(101,200):
	time.sleep(0.05)
	spo2_plot.addDataPoint(i, [99,96])	

# all done, wait for the user to hit the exit button on the plot
spo2_plot.close()
print "Exiting program.  . ."
