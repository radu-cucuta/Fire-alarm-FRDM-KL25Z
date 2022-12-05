import numpy as np
import matplotlib.pyplot as plt
from matplotlib.collections import LineCollection

x    = np.linspace(0,1, 100)
y    = np.linspace(0,1, 100)
cols = np.linspace(0,1,len(x))


while(True):
    a=x[0]
    b=y[0]
    x[0:99]=x[1:100]
    y[0:99]=y[1:100]
    x[99]=a
    y[99]=b    
    points = np.array([x, y]).T.reshape(-1, 1, 2)
    segments = np.concatenate([points[:-1], points[1:]], axis=1)
    
    fig, ax = plt.subplots()
    lc = LineCollection(segments, cmap='viridis')
    lc.set_array(cols)
    lc.set_linewidth(2)
    line = ax.add_collection(lc)
    fig.colorbar(line,ax=ax)
    plt.show()