import numpy as np
import matplotlib.pyplot as plt
import os

logFile = 'UxFinalRes_0'
data = np.loadtxt(os.path.join('logs',logFile))
plt.plot(data[:,0],data[:,1])
plt.xlabel('Simulation time (s)')
plt.ylabel(logFile)
plt.savefig(logFile+'.png')