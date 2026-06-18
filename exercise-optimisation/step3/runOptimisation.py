import matplotlib.pyplot as plt
from platypus import NSGAII, Problem, Real, Integer
import plotly.graph_objects as go
import numpy as np
from pathlib import Path
import sys
import subprocess

CASECOUNT = 1

def runCase(X): #function of 2 variables, X[0] and X[1]
    global CASECOUNT
    alpha = X[0] # angle in degrees
    nx = np.cos(np.radians(alpha)) # x component of normal vector
    ny = np.sin(np.radians(alpha)) # y component of normal vector
    L = X[1]/1000 # length in m

    print(f"Running case {CASECOUNT} with alpha={alpha} degrees and L={L} m")

    path_case = Path(f"case_{CASECOUNT:04d}")
    path_template = Path("heatedPlate_template")

    path_template.copy(path_case, preserve_metadata=True) # copy the template case to a new case folder
    path_snappyHexMeshDict = path_case / "system/snappyHexMeshDict.plate"
    content = path_snappyHexMeshDict.read_text() # read the content of the snappyHexMeshDict.plate file
    content = content.replace("{nx}", str(nx)) # replace the placeholder with the actual angle value
    content = content.replace("{ny}", str(ny)) # replace the placeholder with the actual angle value
    content = content.replace("{L}", str(L)) # replace the placeholder with the actual length
    path_snappyHexMeshDict.write_text(content) # write the modified content back to the file

    subprocess.call(["./Allrun"], cwd=path_case) # run the case in path_case using the Allrun script

    data_pInlet = np.loadtxt(path_case / "postProcessing/pInlet/0/surfaceFieldValue.dat", skiprows=5) # read the pressure at inlet from the file
    data_TbulkOutlet = np.loadtxt(path_case / "postProcessing/TbulkOutlet/0/surfaceFieldValue.dat", skiprows=6) # read the temperature at outlet from the file

    mean_pInlet = np.mean(data_pInlet[-100:,1]) # calculate the mean pressure at inlet
    mean_TbulkOutlet = np.mean(data_TbulkOutlet[-100:,1]) # calculate the mean temperature at outlet

    np.savetxt(path_case / "objectives.txt", [mean_pInlet, mean_TbulkOutlet]) # save the objective values to a text file
    np.savetxt(path_case / "variables.txt", [alpha, L]) # save the variables to a text file


    CASECOUNT += 1 # increment the case count for the next run
    return [mean_pInlet, mean_TbulkOutlet] # return the objective values as a list

# var0: angle (deg)
# var1: length of plate (mm)

# obj0: pressure at inlet (minimize)
# obj1: temperature at outlet (maximize)

n_var = 2
n_obj = 2

problem = Problem(n_var, n_obj) # n_var decision variables, n_obj objectives
problem.directions[0] = Problem.MINIMIZE #minimise pressure at inlet
problem.directions[1] = Problem.MAXIMIZE #maximise pressure at outlet
problem.types[0] = Integer(-90,0)
problem.types[1] = Integer(1,10)

problem.function = runCase # function to evaluate the objectives

algorithm = NSGAII(problem, population_size=20) # NSGAII algorithm for multi-objective optimization
algorithm.run(100) # run the optimization for 10000 iterations

results = algorithm.result # get the results of the optimization
f1 = [s.objectives[0] for s in results] # get the first objective values
f2 = [s.objectives[1] for s in results] # get the second objective values
angle = [problem.types[0].decode(s.variables[0]) for s in results] # get the angle values
length = [problem.types[1].decode(s.variables[1]) for s in results] # get the length values

fig = go.Figure(go.Parcoords(
    line=dict(color=f1, showscale=True, colorscale='Viridis',colorbar=dict(title='Objective 1 (minimize)')),
    dimensions=list([
        dict(label='Angle (deg)', values=angle),
        dict(label='Length (mm)', values=length),
        dict(label='Objective 1 (minimize)', values=f1),
        dict(label='Objective 2 (maximize)', values=f2)
    ])
))
fig.update_layout(title='Parallel Coordinates Plot for Multi-objective Optimization Results')
fig.show()


# plt.plot(f1, f2, 'o') # plot the objective values
# plt.xlabel('Objective 1 (minimize)')
# plt.ylabel('Objective 2 (maximize)')
# plt.title('Pareto Front')
# plt.savefig('pareto_front.png') # save the plot as an image