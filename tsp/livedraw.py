
import numpy as np
import matplotlib
matplotlib.use('TKAgg')
import matplotlib.pyplot as plt
import matplotlib.animation as animation

def data_gen():
    t = data_gen.t
    cnt = 0
    while cnt < 1000:
        cnt+=1
        t += 0.05
        yield t, np.sin(2*np.pi*t) * np.exp(-t/10.)

data_gen.t = 0
fig, ax = plt.subplots()
line, = ax.plot([], [], lw=2)
ax.set_ylim(-1.1, 1.1)
ax.set_xlim(0, 5)
ax.grid()
xdata, ydata = [], []
def run(data):
    # update the data
    t,y = data
    xdata.append(t)
    ydata.append(y)
    xmin, xmax = ax.get_xlim()

    if t >= xmax:
        ax.set_xlim(xmin, 2*xmax)
        ax.figure.canvas.draw()
    line.set_data(xdata, ydata)

    return line,

ani = animation.FuncAnimation(fig, run, data_gen, blit=True, interval=10, repeat=False)
plt.show()

exit(1)

import matplotlib
import matplotlib.pyplot as plt
import random
import time
import itertools
import numpy as np

import threading
import Queue

City = complex # Constructor for new cities, e.g. City(300, 400)
alltours = itertools.permutations


class DrawWorker:
    def __init__(self):
        X = [10,1,2,3,4,5]
        Y = [10,4,3,2,1,0]

        plt.ion()
        self.graph = plt.plot(X,Y)[0]
        plt.show()

        while True:
            Y, X = drawQueue.get()
            self.graph.set_ydata(Y)
            self.graph.set_xdata(X)
            plt.draw()
            drawQueue.task_done()


        plt.plot(X, Y, style)


drawQueue = Queue.Queue()
t = threading.Thread(target=DrawWorker)
t.daemon = True
t.start()

for x in range(50):
    Y = [ random.randint(1,10) for y in range(10)]
    X = [ random.randint(1,10) for x in range(10)]
    drawQueue.put((Y, X))
    time.sleep(2)

def greedy_TSP(cities):
    "At each step, visit the nearest neighbor that is still unvisited."
    start = first(cities)
    tour = [start]
    unvisited = cities - {start}
    while unvisited:
        C = nearest_neighbor(tour[-1], unvisited)
        tour.append(C)
        unvisited.remove(C)
    return tour

time.sleep(5)

drawQueue.join()                    # block until all tasks are done
