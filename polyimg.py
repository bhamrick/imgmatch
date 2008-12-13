import random, copy
from PyQt4 import QtGui, QtCore

class poly:
	def __init__(self, width, height, nsides=3):
		self.x = []
		self.y = []
		self.r = random.random()*255
		self.g = random.random()*255
		self.b = random.random()*255
		self.a = random.random()*255
		self.width = width
		self.height = height
		self.maxmove = 10
		for i in range(nsides):
			self.x.append(random.random()*width)
			self.y.append(random.random()*height)
	def mutate(self):
		for i in range(len(self.x)):
			self.x[i]+=(random.random()-0.5)*2*self.maxmove
		for i in range(len(self.y)):
			self.y[i]+=(random.random()-0.5)*2*self.maxmove
		self.r+=(random.random()-0.5)*2*self.maxmove
		self.g+=(random.random()-0.5)*2*self.maxmove
		self.b+=(random.random()-0.5)*2*self.maxmove
		self.a+=(random.random()-0.5)*2*self.maxmove
		self.r=min(255,max(0,self.r))
		self.g=min(255,max(0,self.g))
		self.b=min(255,max(0,self.b))
		self.a=min(255,max(0,self.a))

class polyimg():
	def __init__(self, width, height, npoly, nvert, target):
		self.width = width
		self.height = height
		self.poly=[]
		self.pmut=0.1
		self.err=0
		for i in range(npoly):
			self.poly.append(poly(width,height,nvert))
	def crossover(self, other):
		cp = int(random.random()*len(self.poly))
		self.poly = copy.deepcopy(self.poly[:cp]) + copy.deepcopy(self.poly[cp:])
	def mutate(self):
		for i in range(len(self.poly)):
			if random.random()<self.pmut:
				self.poly[i].mutate()
	def compute(self,target):
		ans = 0.0
		pm = QtGui.QPixmap(self.width,self.height)
		paint = QtGui.QPainter()
		paint.begin(pm)
		for poly in self.poly:
			qp = QtGui.QPolygonF(len(poly.x))
			for i in range(len(poly.x)):
				qp[i]=QtCore.QPointF(poly.x[i],poly.y[i])
			paint.setBrush(QtGui.QColor(poly.r,poly.g,poly.b,poly.a))
			paint.setPen(QtGui.QColor(poly.r,poly.g,poly.b,poly.a))
			paint.drawPolygon(qp)
		paint.end()
		a = target
		b = pm.toImage()
		for x in range(a.width()):
			for y in range(a.height()):
				ans += 0.5*(((a.pixel(x,y)&0xFF)-(b.pixel(x,y)&0xFF))**2 \
					+ (((a.pixel(x,y)&0xFF00)>>8)-((b.pixel(x,y)&0xFF00)>>8))**2 \
					+ (((a.pixel(x,y)&0xFF0000)>>16)-((b.pixel(x,y)&0xFF0000)>>16))**2)
		self.err = ans
	def save(self,filename):
		pm = QtGui.QPixmap(self.width,self.height)
		paint = QtGui.QPainter()
		paint.begin(pm)
		for poly in self.poly:
			qp = QtGui.QPolygonF(len(poly.x))
			for i in range(len(poly.x)):
				qp[i]=QtCore.QPointF(poly.x[i],poly.y[i])
			paint.setBrush(QtGui.QColor(poly.r,poly.g,poly.b,poly.a))
			paint.setPen(QtCore.Qt.NoPen)
			paint.drawPolygon(qp)
		paint.end()
		pm.toImage().save(filename)
	def toImage(self):
		pm = QtGui.QPixmap(self.width,self.height)
		paint = QtGui.QPainter()
		paint.begin(pm)
		for poly in self.poly:
			qp = QtGui.QPolygonF(len(poly.x))
			for i in range(len(poly.x)):
				qp[i]=QtCore.QPointF(poly.x[i],poly.y[i])
			paint.setBrush(QtGui.QColor(poly.r,poly.g,poly.b,poly.a))
			paint.setPen(QtCore.Qt.NoPen)
			paint.drawPolygon(qp)
		paint.end()
		return pm.toImage()
