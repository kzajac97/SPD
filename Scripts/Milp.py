from ortools.linear_solver import pywraplp
from ortools.sat.python import cp_model
from pathlib import Path
import collections
from Process import *

def Milp(processes):
	horizon = sum([sum(process.times) for process in processes])

	# solver = pywraplp.Solver('simple_mip_program',pywraplp.Solver.CBC_MIXED_INTEGER_PROGRAMMING)
	solver = pywraplp.Solver('simple_mip_program',pywraplp.Solver.GLOP_LINEAR_PROGRAMMING)
	
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
	solver.Parameters = max_time_in_seconds = 120.0
	solver.Minimize(cmax) 
	status = solver.Solve()

	return int(solver.Objective().Value())

def cp_solve(processes):
	model = cp_model.CpModel()
	horizon = sum([sum(process.times) for process in processes])
	# solver = cp_model.CpSolver()
	
	task_type = collections.namedtuple('task_type','start end interval')
	assigned_task_type = collections.namedtuple('assigned_task_type','start job index duration')

	starts = []
	#machine_to_intervals = collections.defaultdict(list)
	machine_to_intervals = []

	for i in range(len(processes)):
		# for j in range(len(processes[i].times)):
		suffix = '_' + str(i)
		start_var = model.NewIntVar(0,horizon,'start' + suffix)
		starts.append(start_var)
		end_var = model.NewIntVar(0,horizon,'end' + suffix)
		interval_var = model.NewIntervalVar(start_var,processes[i].times[1],end_var,'interval' + suffix)

		#all_tasks[i,j] = task_type(start=start_var,end=end_var,interval=interval_var)
		#machine_to_intervals[processes[i].id].append(interval_var)
		machine_to_intervals.append(interval_var)

	#for process in range(len(processes)):
	model.AddNoOverlap(machine_to_intervals)

	for i in range(1,len(processes)):
		model.Add(starts[i] >= processes[i].times[0])
		# model.Add(starts[i] >= starts[i-1])

	# obj_var = model.NewIntVar(0, horizon, 'makespan')
	cmax = model.NewIntVar(0,horizon,'cmax')
	# for i in range(len(processes)):
	# 	model.Add(all_tasks[i,0].end <= all_tasks[i,1].start)
	# 	model.Add(all_tasks[i,1].end <= all_tasks[i,2].start)

	# model.AddMaxEquality(obj_var, [ all_tasks[i,1].end for i in range(len(processes))])

	for i in range(len(processes)):
		model.Add(cmax >= starts[i] + processes[i].times[1] + processes[i].times[2])
	model.Minimize(cmax)

	# Solve model
	solver = cp_model.CpSolver()
	status = solver.Solve(model)

	# return cmax final value
	return int(solver.ObjectiveValue())

def relu(x):
	if x >= 0:
		return x
	else:
		return 0

def witi(processes):
	horizon = sum([sum(process.times) for process in processes])

	# solver = pywraplp.Solver('simple_mip_program',pywraplp.Solver.CBC_MIXED_INTEGER_PROGRAMMING)
	solver = pywraplp.Solver('simple_mip_program',pywraplp.Solver.GLOP_LINEAR_PROGRAMMING)
	
	# create varialbes
	alphas_dict = {}
	starts = []
	delays = []
	for i in range(len(processes)):
		for j in range(len(processes)):
			alphas_dict[i,j] = solver.IntVar(0,1,'alpha_%i_%i' % (i,j))
		
	for i in range(len(processes)):
		starts.append(solver.IntVar(0,horizon,'start_%i' % i))
		delays.append(solve.IntVar(0,max(0,processes[i].times[2] - (starts[i] + processes[i].times[0]),'delay_%i' % i )

	# constraints
	#for i in range(len(processes)):
		# solver.Add(starts[i] >= processes[i].times[0])
		# solver.Add(starts[i] + processes[i].times[0] <= processes[i].times[2])

	for i in range(len(processes)):
		for j in range(i+1, len(processes)):
			solver.Add(starts[i] + processes[i].times[0] <= starts[j] +
			alphas_dict[i,j] * horizon)

			solver.Add(starts[j] + processes[j].times[0] <= starts[i]
			+ alphas_dict[j,i] * horizon)

			solver.Add(alphas_dict[i,j] + alphas_dict[j,i] == 1)

	for i in range(len(processes)):
		solver.Add(delays[i] >= 0)

	# solver
	print('Solving')
	solver.Parameters = max_time_in_seconds = 120.0
	solver.Minimize(sum(delays))
	status = solver.Solve()

	return int(solver.Objective().Value())