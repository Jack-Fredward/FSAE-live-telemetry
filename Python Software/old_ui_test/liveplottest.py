
import serial
import datetime as dt
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style

ser = serial.Serial('COM3', 115200)
# ser = serial.Serial('/dev/ttyACM0',115200)
ser.close()
ser.open()

style.use('fivethirtyeight')

fig = plt.figure()
ax = fig.add_subplot(1,1,1)
xs = []
ys=[]

for i in range(10):
    ser.readline().decode('utf-8')

def animate(i, xs, ys):
    raw_data = ser.readline().decode('utf-8')
    # print(raw_data)
    raw_coolant_temp = raw_data[15:17]
    # print(raw_coolant_temp)
#    # print(raw_coolant_temp.hex())
    # print(int(raw_coolant_temp,16))
    try:
        raw_coolant_temp= int(raw_coolant_temp,16)
    except:
        print("error")
        raw_coolant_temp=0    
    # print(raw_coolant_temp)
    # coolant_tempC = ((raw_coolant_temp*10)/1)+-400
    # coolant_tempF=(coolant_tempC*(9/5))+32
    coolant_tempC = (raw_coolant_temp*0.1)
    coolant_tempF=(coolant_tempC/6.895)


    xs.append(dt.datetime.now().strftime('%H:%M:%S.%f'))
    ys.append(coolant_tempF)

    # Limit x and y lists to 20 items
    xs = xs[-20:]
    ys = ys[-20:]

    # Draw x and y lists
    ax.clear()
    ax.plot(xs, ys)

    # Format plot
    plt.xticks(rotation=45, ha='right')
    plt.subplots_adjust(bottom=0.30)
    plt.title('Live Data test with coolant temp')
    plt.ylabel('Temperature (deg C)')

# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, fargs=(xs, ys), interval=100)
plt.show()
