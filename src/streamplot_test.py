''' stream-plot is a data visualization tool for streaming data.
    Copyright (C) 2013  Sagar G V
    E-mail : sagar.writeme@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
'''

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

# all done, wait for the user to hit the exit button on the displayed plot
spo2_plot.close()
print "Exiting program.  . ."
