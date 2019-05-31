from ortools.linear_solver import pywraplp
from pathlib import Path
from Process import *

def Milp(processes):
    horizon = sum([sum(process.times) for process in processes])

    solver = pywraplp.Solver('simple_mip_program',pywraplp.Solver.CBC_MIXED_INTEGER_PROGRAMMING)
    
    # create varialbes
    alphas_dict = {}
    starts = []

    for i in range(len(processes)):
        for j in range(len(processes)):
            alphas_dict[i,j] = solver.IntVar(0,1,'alpha_%i_%i' % (i,j))
        
    for i in range(len(processes)):
        starts.append(solver.IntVar(0,horizon,'start_%i' % i))

    cmax = solver.IntVar(0,horizon,'cmax')

    # constraints
    for i in range(len(processes)):
        solver.Add(starts[i] >= processes[i].times[0])
        solver.Add(cmax >= starts[i] + processes[i].times[1] + processes[i].times[2])

    for i in range(len(processes)):
        for j in range(i+1, len(processes)):
            solver.Add(starts[i] + processes[i].times[1] <= starts[j] +
            alphas_dict[i,j] * horizon)

            solver.Add(starts[j] + processes[j].times[1] <= starts[i]
            + alphas_dict[j,i] * horizon)

            solver.Add(alphas_dict[i,j] + alphas_dict[j,i] == 1)

    # solver
    print('Solving')
    solver.Minimize(cmax) 
    status = solver.Solve()

    print("makespan", solver.Objective().Value())