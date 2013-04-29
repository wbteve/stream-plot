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
from galry import *
import threading

class StreamPlot():
	def __init__(self,saveFileNameStart = "test",lineColors = ['b']):
		self.n = len(lineColors)
		self.auto_focus = True
		self.vals_to_add = []
		self.npts = 0
		self.auto_t = 10.0
		
		self.vals = [] # each element of this list corresponds to a line plot
		for i in range(self.n):
			self.vals.append(np.array([ [] ])) # each point of this array is a 2-element list [t,yval]
		self.vals_to_add_lock = threading.Lock()
		
		self.ylo = -1.0
		self.yhi = 1.0
		xlim(-1.0,1.0)
		ylim(-1.,1.0)
		
		grid()
		
		for i in range(self.n):
			plot(np.array([]),np.array([]),lineColors[i],name='im'+str(i))
		
		animate(self.anim, dt=.025)
		
		action('KeyPress', 'ToggleAutoFocus', key='A')
		event('ToggleAutoFocus', self.toggleAutoFocus)
		
		self.disp_thread = threading.Thread(target=show)
		self.disp_thread.start()
		#show()
	
	def anim(self,fig,params):
	
		self.vals_to_add_lock.acquire()
		for i in self.vals_to_add:
			self.npts += 1
			t = i[0]
			self.last_t = t
			#assert(len(i[1]) == self.n,"Too few yvals passed")
			for j in range(len(i[1])):
				if self.vals[j].size > 0:
					self.vals[j] = np.vstack((self.vals[j],np.array([t,i[1][j]])))
				else:
					self.vals[j] = np.array([t,i[1][j]])
		self.vals_to_add = []
		self.vals_to_add_lock.release()
		
		for i in range(self.n):
			fig.set_data(visual='im'+str(i),position=self.vals[i])
		
		#fig.set_data(visual='im1',position=np.array([ [1,97],[2,99] ]))
		
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
			
			if ylo == -float('Inf'):
				ylo = -1.0
			if yhi == float('Inf'):
				yhi = 1.0
			
			if self.ylo < 0.5*ylo or self.ylo > 0.9*ylo:
				self.ylo = 0.7*ylo
			if self.yhi > 1.5*yhi or self.yhi < 1.1*yhi:
				self.yhi = 1.3*yhi
			
			fig.process_interaction('SetViewbox', [xstart,self.ylo,xstop,self.yhi ])
		
	def addDataPoint(self,t,val_list): 
		self.vals_to_add_lock.acquire()
		self.vals_to_add.append( (t,val_list) )
		self.vals_to_add_lock.release()
		
	def toggleAutoFocus(self,fig,params):
		self.auto_focus = not self.auto_focus
		
	def close(self):
		self.disp_thread.join()


