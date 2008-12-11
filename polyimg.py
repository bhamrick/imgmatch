import random
from PyQt4 import QtGui, QtCore

class poly:
	def __init__(self, width, height, nsides=3):
		self.x = []
		self.y = []
		self.width = width
		self.height = height
		self.pnew = 0.05
		self.maxmove = 5
		for i in range(nsides):
			self.x.append(random.random()*width)
			self.y.append(random.random()*height)
	def mutate(self):
		for i in range(len(self.x)):
			self.x[i]+=(random.random()-0.5)*2*self.maxmove
		for i in range(len(self.y)):
			self.y[i]+=(random.random()-0.5)*2*self.maxmove

class polyimg:
	def __init__(self, width, height, npoly, nvert):
		self.width = width
		self.height = height
		self.poly=[]
		self.pmut=0.1
		for i in range(npoly):
			self.poly.append(poly(width,height,nvert))
	def crossover(self, other):
		cp = int(random.random()*len(self.poly))
		self.poly = self.poly[:cp] + self.poly[cp:]
	def mutate(self):
		for i in range(len(self.poly)):
			if random.random()<self.pmut:
				self.poly[i].mutate()
	def error(self, target):
		ans = 0.0
