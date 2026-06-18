import matplotlib.pyplot as plt
from platypus import NSGAII, Problem, Real, Integer
import plotly.graph_objects as go

def schaffer(x): #function of 2 variables, x[0] and x[1]
    return [x[0]+x[1]**2, (x[0]-2)**2]

# var0: angle (deg)
# var1: length of plate (mm)

n_var = 2
n_obj = 2

problem = Problem(n_var, n_obj) # n_var decision variables, n_obj objectives
problem.directions[0] = Problem.MINIMIZE #minimise pressure at inlet
problem.directions[1] = Problem.MAXIMIZE #maximise pressure at outlet
problem.types[0] = Integer(-90,0)
problem.types[1] = Integer(1,10)

problem.function = schaffer # function to evaluate the objectives

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