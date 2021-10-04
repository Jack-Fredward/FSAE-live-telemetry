import tkinter as tk
from tkinter import *
from tkinter import ttk
from typing import Container
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


"""
TODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODO
TODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODO
TODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODO
TODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODO
TODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODO
TODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODO
TODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODO
TODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODO
TODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODO
TODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODO

format
format
format

"""



# tk styling
LARGEFONT =("Verdana", 35) 
SMALLFONT =("calibre",10)
# matplotlib graph styling
# style.use("")
style.use("bmh")

#format the graph with the given ylabel and ylimit axis
def formatGraph(fig, yLabel, yLim):
	fig.set_ylabel(yLabel)
	fig.set_ylim(yLim)

#update the data to be displayed placeholder for now
def updateData():
	file = open("sampleData.txt", "r+")
	x=len(file.readlines())+1
	y = randint(0, 10)
	file.write(str(x)+','+str(y)+'\n')
	file.close()
	return x

#animate the graphs (make them live)
#gets data and appends to display lists then plots and formats
def animate(i, app):
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
	i=0
	colors = ['b', 'g', 'r', 'k', 'y','b', 'g', 'r', 'k',]
	markers = ['o-','.-','o--','.--','s-','s:','*--','|-','_-']
	for subplot in app.f.get_axes():
		
		subplot.cla()
		subplot.plot(xList[xValue-10:],yList[xValue-10:],colors[i]+markers[i])
		# subplot.plot(xList,yList,colors[i]+markers[i])
		i=i+1
	
	#if i put all the titles in a list then i can just use the for loop above to do all the formatting
	#format Graphs
	formatGraph(app.ax1, "Ax1's \ngraph \ny label", (-1,11))
	formatGraph(app.ax2, "Ax2's \ngraph \ny label", (0,10))
	formatGraph(app.ax3, "Ax3's \ngraph \ny label", (0,10))
	formatGraph(app.ax4, "Ax4's \ngraph \ny label", (-5,5))
	formatGraph(app.ax5, "Ax5's \ngraph \ny label", (0,10))
	formatGraph(app.ax6, "Ax6's \ngraph \ny label", (0,5))
	formatGraph(app.ax7, "Ax7's \ngraph \ny label", (0,10))
	formatGraph(app.ax8, "Ax8's \ngraph \ny label", (5,10))
	formatGraph(app.ax9, "Ax9's \ngraph \ny label", (0,10))
	app.ax9.set_xlabel("Data Point")
	# app.ax9.set_xticks(rotation=45)
	plt.setp(app.ax9.get_xticklabels(), rotation=45, ha="right",
         rotation_mode="anchor")


class LiveTelemUI(tk.Tk):
	# __init__ function for class tkinterApp 
	def __init__(self):
		# __init__ function for class Tk 
		tk.Tk.__init__(self)
		tk.Tk.wm_title(self, "Live Telemetry")

		#figure
		self.f = Figure()
		self.f, ((self.ax1, self.ax2, self.ax3, self.ax4,self.ax5, self.ax6, self.ax7, self.ax8, self.ax9)) = plt.subplots(9,1, sharex=True)

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


		#graph selection
		minimap = IntVar(value=1)
		darkmode = IntVar(value=1)

		view = Menu(menubar, tearoff=0)
		view.add_checkbutton(label="show minimap", onvalue=1, offvalue=0, variable=minimap, command= lambda: str(minimap.get()))
		view.add_checkbutton(label="Darkmode", onvalue=1, offvalue=0, variable=darkmode, command= lambda: str(darkmode.get()))
		menubar.add_cascade(label='View', menu=view)
		# view.index("show minimap").select()
		# self.chkmenu.invoke(self.chkmenu.index('C'))
		# view.activate(view.index("show minimap"))
		# view.activate(view.index("Darkmode"))



		tk.Tk.config(self, menu=menubar)
		# view.invoke(view.index("show minimap"))

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
		#displaying the graph in window, pack to allow for just the graph and expand and fill to do just that to the window
		canvas = FigureCanvasTkAgg(controller.f, self)
		canvas.draw()
		canvas.get_tk_widget().pack(side=TOP, fill=BOTH, expand=1)




#main
def main():
	app = LiveTelemUI()
	app.geometry("1280x760")
	#start in fullscreen windowed
	# app.state('zoomed')
	ani = animation.FuncAnimation(app.f, animate,fargs = (app,),interval=500)
	app.mainloop()
	

main()