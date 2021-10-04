import tkinter as tk
from tkinter import ttk
import matplotlib
from matplotlib import figure
matplotlib.use("TkAgg")
from matplotlib.backends.backend_tkagg import FigureCanvasAgg, FigureCanvasTkAgg, NavigationToolbar2Tk
from matplotlib.figure import Figure
import matplotlib.animation as animation
from matplotlib import style
import matplotlib.pyplot as plt
import numpy as np
from random import seed
from random import *
import time

LARGEFONT =("Verdana", 35) 
SMALLFONT =("calibre",10)
#style.use("ggplot")
f = Figure()
a = f.add_subplot(111)
seed(time.time())
def updateData():
	file = open("sampleData.txt", "r+")
	x=len(file.readlines())+1
	print(x)
	y = randint(0, 10)
	file.write(str(x)+','+str(y)+'\n')
	return x

	


def animate(i):
	xValue = updateData()
	pullData = open("sampleData.txt","r").read()
	dataList = pullData.split('\n')
	xList = []
	yList = []
	for eachLine in dataList:
		if len(eachLine)>1:
			x, y = eachLine.split(',')
			xList.append(int(x))
			yList.append(int(y))
	
	a.clear()
	a.plot(xList[xValue-10:],yList[xValue-10:])


class LiveTelemUI(tk.Tk):
	# __init__ function for class tkinterApp 
	def __init__(self):
		# __init__ function for class Tk 
		tk.Tk.__init__(self)
		tk.Tk.wm_title(self, "Live Telemetry")

		# creating a container 
		container = tk.Frame(self) 
		container.pack(side = "top", fill = "both", expand = True) 
		container.grid_rowconfigure(0, weight = 1) 
		container.grid_columnconfigure(0, weight = 1)

		# creating a menu bar
		menubar = tk.Menu(container)

		#file menu
		filemenu = tk.Menu(menubar, tearoff=0)
		filemenu.add_command(label="Exit", command = quit)
		menubar.add_cascade(label="File", menu=filemenu)

		tk.Tk.config(self, menu=menubar)


		# initializing frames to an empty array 
		self.frames = {}

		frame = StartPage(container, self)

		self.frames[StartPage] = frame

		frame.grid(row = 0, column = 0, sticky ="nsew")

		self.show_frame(StartPage)

	def show_frame(self, cont):
		frame = self.frames[cont]
		frame.tkraise()

class StartPage(tk.Frame):
	def __init__(self, parent, controller):
		tk.Frame.__init__(self, parent)
		mainLabel = ttk.Label(self, text = "StartPage", font = LARGEFONT)
		mainLabel.pack(side=tk.TOP, fill=tk.BOTH, expand=True)


		#dont need the toolbar i can use grid

		#displaying the graph in window
		canvas = FigureCanvasTkAgg(f, self)
		canvas.draw()
		canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=True)

		toolbar = NavigationToolbar2Tk(canvas, self)
		toolbar.update()
		canvas._tkcanvas.pack(side=tk.TOP, fill=tk.BOTH, expand=True)


def main():
	app = LiveTelemUI()
	app.geometry("1280x720")
	ani = animation.FuncAnimation(f, animate, interval=500)
	app.mainloop()

main()