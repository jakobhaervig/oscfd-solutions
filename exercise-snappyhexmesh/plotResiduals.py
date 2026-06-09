import numpy as np
import matplotlib.pyplot as plt
import os
logFiles=os.listdir('logs')

for logFile in logFiles:
    try:
        data=np.loadtxt(os.path.join('logs',logFile))
        plt.plot(data[:,0],data[:,1])
        plt.xlabel('Simulation time (s)')
        plt.ylabel(logFile)
        plt.savefig('logs/'+logFile+'.pdf')
        print('Processed',logFile)
    except:
        print('Failed to process',logFile)
