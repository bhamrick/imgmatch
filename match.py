#!/usr/bin/env python
from processing import Process
from processing.sharedctypes import Value, Array, synchronized
from ctypes import Structure, c_double
from polyimg import *
from PyQt4 import QtGui, QtCore
import sys,copy,time

app = QtGui.QApplication(sys.argv)

class display(QtGui.QWidget):
	def updateImage(self,img):
		self.img = img
	def paintEvent(self,event):
		paint = QtGui.QPainter()
		paint.begin(self)
		paint.drawImage(QtCore.QPoint(0,0),self.img)
		paint.end()

filename = sys.argv[1]
target = QtGui.QImage(filename)

class galg:
	def __init__(self,tgt):
		self.app = QtGui.QApplication(sys.argv)
		self.npop = 16
		self.npoly = 5
		self.nvert = 4
		self.pmut = 0.2
		self.gen = 0
		self.target = tgt
		self.running=0
		
		self.population = []
		for i in range(self.npop):
			self.population.append(polyimg(self.target.width(),self.target.height(),self.npoly,self.nvert,self.target))
		
		self.disp = display()
		self.disp.setFixedSize(self.target.width(),self.target.height())
		self.disp.updateImage(self.population[0].toImage())
		self.disp.show()
		
		self.timer = QtCore.QTimer()
		self.timer.setSingleShot(False)
		QtCore.QObject.connect(self.timer,QtCore.SIGNAL("timeout()"),self.run)
		self.timer.start(100)
	def run(self):
		if self.running==1:
			return
		self.running = 1
		self.gen += 1
		tpop = []
		for i in range(self.npop):
			self.population[i].compute(target)
			tpop.append([self.population[i].err,self.population[i]])
		l = sorted(tpop)
		l[0][1].save("out.jpg")
		print self.gen, '\t', l[0][0]
		best = copy.deepcopy(l[0][1])
		self.disp.updateImage(l[0][1].toImage())
		self.disp.update()
		nxpop = [copy.deepcopy(l[0][1])]
		for i in range(self.npop-1):
			x = random.random()
			v = 0.5
			a = 0
			while x < v and a < self.npop-1:
				v *= 0.5
				a += 1
			x = random.random()
			v = 0.5
			b = 0
			while x < v and b < self.npop-1:
				v *= 0.5
				b += 1
			nxpop.append(copy.deepcopy(l[a][1]))
			nxpop[i+1].crossover(copy.deepcopy(l[b][1]))
		for i in range(self.npop):
			if random.random() < self.pmut:
				nxpop[i].mutate()
		self.population = nxpop
		self.population[0] = copy.deepcopy(best)
		self.running=0

g = galg(target)
app.exec_()
