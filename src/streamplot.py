''' stream-plot is a data visualization tool for streaming data. It is based on Galry by Cyrille Rossant.
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
from galry import *
import threading
import glob
import re

class StreamPlot():
	def __init__(self,saveFileNameStart = "test",lines = [('l','g','plotName')],nSamples=100,auto_t=10.0,legend = False):
		self.saveFileNameStart = saveFileNameStart 
		self.nSamples = nSamples
		self.n = len(lines)
		self.auto_focus = True
		self.vals_to_add = []
		self.npts = 0
		self.auto_t = auto_t
		self.last_t = 0.0
		self.vals = [ np.array([]) for i in range(self.n) ]
		self.vals_to_add_lock = threading.Lock()
		self.lines = lines
		
		self.ylo = -1.0
		self.yhi = 1.0
		xlim(-1.0,1.0)
		ylim(-1.,1.0)
		
		grid()
		
		for i in range(self.n):
			plot(np.array([]),np.array([]),lines[i][0],color=lines[i][1],name='im'+str(i))
			text(lines[i][2],color=self.letterToRGBA(lines[i][1]),name='legend'+str(i), fontsize=16, is_static=True,coordinates=(0.5, 0.95 - i*0.1))
		
		animate(self.anim, dt=.025)
		
		action('KeyPress', 'ToggleAutoFocus', key='A')
		event('ToggleAutoFocus', self.toggleAutoFocus)
		
		action('KeyPress', 'saveLastNSamples', key='D')
		event('saveLastNSamples', self.saveLastNSamples)
		
		action('KeyPress', 'saveAllSamples', key='F')
		event('saveAllSamples', self.saveAllSamples)
		
		action('KeyPress', 'zoomInTime', key='Z')
		event('zoomInTime', self.zoomInTime)
		
		action('KeyPress', 'zoomOutTime', key='X')
		event('zoomOutTime', self.zoomOutTime)
		
		action('KeyPress', 'saveVisibleSamples', key='C')
		event('saveVisibleSamples', self.saveVisibleSamples)
		
		self.disp_thread = threading.Thread(target=show) # display the plot in a new thread so as to make object creation non-blocking
		self.disp_thread.start()
	
	def letterToRGBA(self,s):
		if s == 'r':
			return (1.0,0.0,0.0,1.0)
		elif s == 'g':
			return (0.0,1.0,0.0,1.0)
		elif s == 'b':
			return (0.0,0.0,1.0,1.0)
		elif s == 'y':
			return (1.0,1.0,0.0,1.0)
		else:
			return (1.0,1.0,1.0,1.0)
	
	def anim(self,fig,params):
	
		# remove values in the buffer and put them into an NumPy array suitable for consumption by Galry
		self.vals_to_add_lock.acquire()
		for i in self.vals_to_add:
			self.npts += 1
			t = i[0]
			self.last_t = t
			firstTime = False
			for j in range(len(i[1])):
				if self.vals[j].size > 0:
					self.vals[j] = np.vstack((self.vals[j],np.array([t,i[1][j]])))
				else:
					self.vals[j] = np.array([t,i[1][j]])
					self.vals[j] = np.vstack((self.vals[j],np.array([t,i[1][j]]))) # otherwise, the array type goes bad
					firstTime = True
			if firstTime:
				self.npts += 1
		self.vals_to_add = []
		self.vals_to_add_lock.release()
		
		#print self.vals
		for i in range(self.n):
			fig.set_data(visual='im'+str(i),position=self.vals[i])
			fig.set_data(text=self.lines[i][2]+' : '+str(self.vals[i][-1][1]), visual='legend'+str(i))

		# find out the peak and trough values in the last 'self.auto_t' units of time, pass it through a window comparator and set the view box
		
		if self.auto_focus:
			xstart = self.last_t - self.auto_t
			xstop = self.last_t * 1.0
			ylo = float('Inf')
			yhi = -float('Inf')
			
			
			for i in reversed(range(self.npts)):
				if(self.vals[0][i][0] < xstart):
					break
				ys = []
				for j in range(self.n):
					ys.append(self.vals[j][i][1])
				
				maxval = max(ys)
				minval = min(ys)
				if maxval > yhi:
					yhi = maxval
				if minval < ylo:
					ylo = minval

			if ylo == float('Inf'):
				ylo = -1.0
			if yhi == -float('Inf'):
				yhi = 1.0
			
			if self.ylo <= 0.5*ylo or self.ylo >= 0.9*ylo:
				self.ylo = 0.7*ylo
			if self.yhi >= 1.5*yhi or self.yhi <= 1.1*yhi:
				self.yhi = 1.3*yhi
			
			viewBox = [xstart,self.ylo,xstop,self.yhi ]
			fig.process_interaction('SetViewbox', viewBox)
		
	def addDataPoint(self,t,val_list): 
		self.vals_to_add_lock.acquire()
		self.vals_to_add.append( (t,val_list) )
		self.vals_to_add_lock.release()
		
	def toggleAutoFocus(self,fig,params):
		self.auto_focus = not self.auto_focus
	
	def saveLastNSamples(self,fig,params):
		self.saveNsamples(self.nSamples)
	
	def saveAllSamples(self,fig,params):
		self.saveNsamples(self.npts)
	
	def zoomOutTime(self,fig,params):
		self.auto_t *= 2.0
	
	def zoomInTime(self,fig,params):
		self.auto_t /= 2.0
	
	def saveVisibleSamples(self,fig,params):
		fileName = self.getNextCsvName()
		(xstart,ystart,xstop,ystop) = fig.get_processor('navigation').get_viewbox()
		with open(fileName,'w') as f:
			for i in range(self.npts):
				t = self.vals[0][i][0]
				if t >= xstart and t <= xstop:
					f.write(str(t))
					for j in range(self.n):
						f.write(',')
						f.write(str(self.vals[j][i][1]))
					f.write('\n')
		print "Saved samples from t = "+str(xstart)+" to t = "+str(xstop)+" in "+fileName
	
	def saveNsamples(self,N):
		fileName = self.getNextCsvName()
		with open(fileName,'w') as f:
			n = 0
			start = self.npts - N
			if start < 0:
				start = 0
			for i in range(start,self.npts):
				n += 1
				if(n >= N):
					break
				f.write(str(self.vals[0][i][0]))
				for c in range(self.n):
					f.write(',')
					f.write(str(self.vals[c][i][1]))
				f.write('\n')
			print "Saved to "+str(fileName)
				
	def getNextCsvName(self):
		fileNo = 0
		existingFileList = glob.glob(self.saveFileNameStart+'*.csv')
		for i in existingFileList:
			s = re.match(r'.*_(\d*)\.csv',i)
			if s:
				num = int(s.group(1))
				if num >= fileNo:
					fileNo = num + 1
		return self.saveFileNameStart+"_"+str(fileNo)+'.csv'
	
	def waitUntilExit(self):
		self.disp_thread.join()
	
	def isPlotAlive(self):
		return self.disp_thread.isAlive()


