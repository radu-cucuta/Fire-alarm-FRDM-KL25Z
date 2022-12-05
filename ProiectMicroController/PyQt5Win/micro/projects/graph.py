from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import tkinter as tk
from matplotlib.collections import LineCollection

import numpy as np
import serial as sr
import pyqtgraph as pg
import matplotlib.pyplot as plt

#------global variables
data = np.array([])
cond = False
mutex = False

#-----plot data-----
def plot_data():
    global cond, data
    
    if (cond == True):
        
        a = s.readline()
        a.decode()
        
        if(len(data) < 100):
            data = np.append(data,float(a[0:4]))
        else:
            data[0:99] = data[1:100]
            data[99] = float(a[0:4])
		
        x=np.arange(0,len(data))
        y=data
        lines.set_xdata(x)
        lines.set_ydata(y)
        lines.set_color('black')
        # plt.show()

        # for x1, x2, y1,y2 in zip(x, x[1:], y, y[1:]):
        #     if y1 > y2:
        #         lines.plot([x1, x2], [y1, y2], 'r')
        #     elif y1 < y2:
        #         lines.plot([x1, x2], [y1, y2], 'g')
        #     else:
        #         lines.plot([x1, x2], [y1, y2], 'b')

        canvas.draw()
    
    root.after(1,plot_data)

def plot_start():
    global cond
    cond = True
    s.reset_input_buffer()

def plot_stop():
    global cond
    cond = False
    
def set_buzzer():
    global mutex
    if mutex:
        s.write(str.encode('1'))
    else:
        s.write(str.encode('0'))
    print("ceva")
    s.reset_input_buffer()

    mutex = not mutex
    

#-----Main GUI code-----
root = tk.Tk()
root.title('Flame sensor graph')
root.configure(background = '#2b303a')
root.geometry("900x500") # set the window size
#------create Plot object on GUI----------
# add figure canvas
fig = Figure()
fig.patch.set_facecolor('#ccdde2')

ax = fig.add_subplot(111)

# x    = np.linspace(0,1, 100)
# y    = np.linspace(0,1, 100)
# points = np.array([x, y]).T.reshape(-1, 1, 2)
# segments = np.concatenate([points[:-1], points[1:]], axis=1)

# cols = np.linspace(0,1,100)
# lc = LineCollection(segments, cmap='viridis')
# lc.set_array(cols)
# lc.set_linewidth(2)
# line = ax.add_collection(lc)
# fig.colorbar(line,ax=ax)

#ax = plt.axes(xlim=(0,100),ylim=(0, 120)); #displaying only 100 samples
ax.set_title('Flame graph');
ax.set_xlabel('Time')
ax.set_ylabel('Threat level flame')
ax.set_xlim(0,100)
ax.set_ylim(-0.5,1200)
ax.axhspan(0,333,facecolor='green',alpha=0.5)
ax.axhspan(333,497,facecolor='yellow',alpha=0.5)
ax.axhspan(497,503,facecolor='blue',alpha=0.5)
ax.axhspan(503,666,facecolor='yellow',alpha=0.5)
ax.axhspan(666,1200,facecolor='red',alpha=0.5)

lines = ax.plot([],[])[0]

canvas = FigureCanvasTkAgg(fig, master=root)  # A tk.DrawingArea.
canvas.get_tk_widget().place(x = 50,y=30, width = 800,height = 400)
canvas.get_tk_widget().configure(bg="black")
canvas.draw()

#----------create button---------
root.update();
start = tk.Button(root, text = "Measure", font = ('calbiri',12),command = lambda: plot_start())
start.place(x = 340, y = 450 )

root.update();
stop = tk.Button(root, text = "Stop", font = ('calbiri',12), command = lambda:plot_stop())
stop.place(x = start.winfo_x()+start.winfo_reqwidth() + 20, y = 450)

root.update();
buzzer = tk.Button(root, text = "BUZZER", font = ('calbiri',12), command = lambda:set_buzzer())
buzzer.place(x = stop.winfo_x()+stop.winfo_reqwidth() + 20, y = 450)


label = tk.Label(root, text='Buzzer Line',bg='#FFFF7F')
label.place(x = 427, y = 245)
#----start serial port----
s = sr.Serial('COM3',115200);
s.reset_input_buffer()



root.after(1,plot_data)
root.mainloop()