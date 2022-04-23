from posixpath import split
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
import serial
import datetime as dt


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
ser = serial.Serial('COM3', 115200)
ser.close()
ser.open()

xList = []

throttle_position_data = []
engine_speed_data = []
fuel_pressure_data = []
engine_oil_pressure_data = []
engine_oil_temp_data = []
coolant_temp_data = []
gear_data = []
exhaust_lambda_data = []
front_brake_pressure_data = []
rear_brake_pressure_data = []

style.use("bmh")

#format the graph with the given ylabel and ylimit axis
def formatGraph(fig, yLabel, yLim):
	fig.set_ylabel(yLabel,ha='right', rotation='horizontal')
	# fig.set_ylim(yLim)

# #update the data to be displayed placeholder for now
# def updateData():
# 	file = open("sampleData.txt", "r+")
# 	x=len(file.readlines())+1
# 	y = randint(0, 10)
# 	file.write(str(x)+','+str(y)+'\n')
# 	file.close()
# 	return x

#animate the graphs (make them live)
#gets data and appends to display lists then plots and formats
def animate(i, xList, throttle_position_data,engine_speed_data,fuel_pressure_data,engine_oil_pressure_data,engine_oil_temp_data,coolant_temp_data,gear_data,exhaust_lambda_data,front_brake_pressure_data,rear_brake_pressure_data, app):
	for i in range(10):
		try:
			raw_data = ser.readline().decode('utf-8')
		except:
			raw_data = ":0"
		
		try:
			split_data = raw_data.split(':')
			split_data[1].replace('\\','')
			split_data[1].replace('r','')
			split_data[1].replace('n','')
		except:
			print("cant split")
		# print(split_data)
		try:
			data_value = float(split_data[1])
		except:
			data_value = 0

		if split_data[0] == "Front Brake Pressure":
			front_brake_pressure_data.append(data_value)
		elif split_data[0] == "Rear Brake Pressure":
			rear_brake_pressure_data.append(data_value)
		elif split_data[0] == "Throttle Position":
			throttle_position_data.append(data_value)
		elif split_data[0] == "Engine Speed":
			engine_speed_data.append(data_value)	
		elif split_data[0] == "Fuel Pressure":
			fuel_pressure_data.append(data_value)
		elif split_data[0] == "Engine Oil Pressure":
			engine_oil_pressure_data.append(data_value)
		elif split_data[0] == "Engine Oil Temp":
			engine_oil_temp_data.append(data_value)
		elif split_data[0] == "Coolant Temp":
			coolant_temp_data.append(data_value)
		elif split_data[0] == "Exhaust Lambda":
			exhaust_lambda_data.append(data_value)
		elif split_data[0] == "Gear":
			gear_data.append(data_value)



	# front_brake_pressure = 0
	# # print(split_data)
	# if split_data[0] == "Front Brake Pressure":
	# 	front_brake_pressure = split_data[1]
	# 	print(split_data[1])
	# 	try:
	# 		front_brake_pressure = float(front_brake_pressure)
	# 	except:
	# 		print("error")
	# 		front_brake_pressure=0

	# 	yList[8].append(front_brake_pressure)
	# 	yList = yList

	# xValue = updateData()
	# pullData = open("sampleData.txt","r").read()
	# dataList = pullData.split('\n')
	# xList = []
	# yList = []
	# for eachLine in dataList:
		# if len(eachLine)>1:
			# x, y = eachLine.split(',')
	xList.append(dt.datetime.now().strftime('%H:%M:%S'))
	# yList.append(front_brake_pressure)
	num_of_datapoints = 10
	xList = xList[-int(num_of_datapoints):]
	# yList = yList[-int(num_of_datapoints):]

	throttle_position_data = throttle_position_data[-int(num_of_datapoints):]
	engine_speed_data = engine_speed_data[-int(num_of_datapoints):]
	fuel_pressure_data = fuel_pressure_data[-int(num_of_datapoints):]
	engine_oil_pressure_data = engine_oil_pressure_data[-int(num_of_datapoints):]
	engine_oil_temp_data = engine_oil_temp_data[-int(num_of_datapoints):]
	coolant_temp_data = coolant_temp_data[-int(num_of_datapoints):]
	gear_data = gear_data[-int(num_of_datapoints):]
	exhaust_lambda_data = exhaust_lambda_data[-int(num_of_datapoints):]
	front_brake_pressure_data = front_brake_pressure_data[-int(num_of_datapoints):]
	rear_brake_pressure_data = rear_brake_pressure_data[-int(num_of_datapoints):]


	data = [throttle_position_data, engine_speed_data, fuel_pressure_data, engine_oil_pressure_data, engine_oil_temp_data, coolant_temp_data, gear_data, exhaust_lambda_data, front_brake_pressure_data, rear_brake_pressure_data]
	# for array in data:
	# 	print(array)


	i=0
	colors = ['b', 'g', 'r', 'k', 'y','b', 'g', 'r', 'k','r']
	markers = ['o-','.-','o--','.--','s-','s:','*--','|-','_-','.--']
	# subplots = app.f.get_axes()
	sensors = ["Throttle Position", "Engine Speed", "Fuel Pressure", "Engine Oil Pressure", "Engine Oil Temp", "Coolant Temp", "Gear", "Exhaust Lambda", "Front Brake \nPressure", "Rear Brake \nPressure"]


	for subplot in app.f.get_axes():
		
		subplot.cla()
		try:
			subplot.plot(xList,data[i],colors[i]+markers[i])
			
			# formatGraph(subplot, str(data[i][-1]))
		except:
			print("cant plot")
		
		try:
			label = sensors[i] + "\n" + str(data[i][-1])
			subplot.set_ylabel(label,ha='right', labelpad=10,rotation='horizontal')
			# subplot.set_ylabel(str(data[i][-1]),ha='right', labelpad=10,rotation='horizontal')
		# 	# subplot.yaxis.set_label_position("right")
		# 	second_y_label = subplot.twinx()
		# 	second_y_label.set_ylabel(str(data[i][-1]),rotation='horizontal')
		# 	# subplot.ylabel(data[i], rotation='horizontal')
		# 	# subplot.yaxis.set_label_position("left")
		except:
			print("cant label")
		
		# subplot.plot(xList,yList,colors[i]+markers[i])
		i=i+1
	

	

	#if i put all the titles in a list then i can just use the for loop above to do all the formatting
	#format Graphs
	# formatGraph(app.ax1, "Throttle Position", (-1,11))
	# formatGraph(app.ax2, "Engine Speed", (0,10))
	# formatGraph(app.ax3, "Fuel Pressure", (0,10))
	# formatGraph(app.ax4, "Engine Oil Pressure", (-5,5))
	# formatGraph(app.ax5, "Engine Oil Temp", (0,10))
	# formatGraph(app.ax6, "Coolant Temp", (0,5))
	# formatGraph(app.ax7, "Gear", (0,10))
	# formatGraph(app.ax8, "Exhaust Lambda", (5,10))
	# formatGraph(app.ax9, "Front Brake Pressure", (0,10))
	# formatGraph(app.ax10, "Rear Brake Pressure", (1,1))
	# formatGraph(app.ax1, "Ax1's \ngraph \ny label", (-1,11))
	# formatGraph(app.ax2, "Ax2's \ngraph \ny label", (0,10))
	# formatGraph(app.ax3, "Ax3's \ngraph \ny label", (0,10))
	# formatGraph(app.ax4, "Ax4's \ngraph \ny label", (-5,5))
	# formatGraph(app.ax5, "Ax5's \ngraph \ny label", (0,10))
	# formatGraph(app.ax6, "Ax6's \ngraph \ny label", (0,5))
	# formatGraph(app.ax7, "Ax7's \ngraph \ny label", (0,10))
	# formatGraph(app.ax8, "Ax8's \ngraph \ny label", (5,10))
	# formatGraph(app.ax9, "Ax9's \ngraph \ny label", (0,10))
	app.ax10.set_xlabel("Time(s)")
	# app.ax9.set_xlabel("Data Point")
	# app.ax9.set_xticks(rotation=45)
	plt.setp(app.ax10.get_xticklabels(), rotation=45, ha="right",
         rotation_mode="anchor")



class LiveTelemUI(tk.Tk):
	# __init__ function for class tkinterApp 
	def __init__(self):
		# __init__ function for class Tk 
		tk.Tk.__init__(self)
		tk.Tk.wm_title(self, "Live Telemetry")

		#figure
		self.f = Figure()
		self.f, ((self.ax1, self.ax2, self.ax3, self.ax4,self.ax5, self.ax6, self.ax7, self.ax8, self.ax9, self.ax10)) = plt.subplots(10,1, sharex=True)

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
	ani = animation.FuncAnimation(app.f, animate,fargs = (xList, throttle_position_data,engine_speed_data,fuel_pressure_data,engine_oil_pressure_data,engine_oil_temp_data,coolant_temp_data,gear_data,exhaust_lambda_data,front_brake_pressure_data,rear_brake_pressure_data, app,),interval=500)
	app.mainloop()
	

main()

