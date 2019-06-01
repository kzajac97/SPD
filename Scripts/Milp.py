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
	
	task_type = collections.namedtuple('task_type','start end interval')
	assigned_task_type = collections.namedtuple('assigned_task_type','start job index duration')

	all_tasks = {}
	machine_to_intervals = collections.defaultdict(list)

	for i in range(len(processes)):
		for j in range(len(processes[i].times)):
			suffix = '_%i_%i' % (i,j)
			start_var = model.NewIntVar(0,horizon,'start' + suffix)
			end_var = model.NewIntVar(0,horizon,'end' + suffix)
			interval_var = model.NewIntervalVar(start_var,processes[i].times[j],end_var,'interval' + suffix)

			all_tasks[i,j] = task_type(start=start_var,end=end_var,interval=interval_var)
			machine_to_intervals[processes[i].id].append(interval_var)

	for process in range(len(processes)):
		model.AddNoOverlap(machine_to_intervals[i])

	for i in range(len(processes)-1):
		model.Add(all_tasks[i,1].start >= all_tasks[i+1,1].end)

	obj_var = model.NewIntVar(0, horizon, 'makespan')

	for i in range(len(processes)):
		model.Add(all_tasks[i,0].end <= all_tasks[i,1].start)
		model.Add(all_tasks[i,1].end <= all_tasks[i,2].start)

	model.AddMaxEquality(obj_var, [ all_tasks[i,1].end for i in range(len(processes))])

	model.Minimize(obj_var)

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

	for i in range(len(processes)):
		for j in range(len(processes)):
			alphas_dict[i,j] = solver.IntVar(0,1,'alpha_%i_%i' % (i,j))
		
	for i in range(len(processes)):
		starts.append(solver.IntVar(0,horizon,'start_%i' % i))

	# constraints
	for i in range(len(processes)):
		solver.Add(starts[i] >= processes[i].times[0])
		solver.Add(starts[i] + processes[i].times[0] <= processes[i].times[2])

	for i in range(len(processes)):
		for j in range(i+1, len(processes)):
			solver.Add(starts[i] + processes[i].times[0] <= starts[j] +
			alphas_dict[i,j] * horizon)

			solver.Add(starts[j] + processes[j].times[0] <= starts[i]
			+ alphas_dict[j,i] * horizon)

			solver.Add(alphas_dict[i,j] + alphas_dict[j,i] == 1)

	# solver
	print('Solving')
	solver.Parameters = max_time_in_seconds = 120.0
	solver.Minimize(sum([processes[i].times[1] * (relu(processes[i].times[2] - (starts[i] + processes[i].times[0]))) for i in range(len(processes))]))
	status = solver.Solve()

	return int(solver.Objective().Value())

def get_jobs(processes):
	data = []
	for process in processes:
		process_data = []
		for i in range(len(process.times)):
			process_data.append((i,process.times[i]))
		data.append(process_data)

	return data

def Jobshop(processes):
	# Create the model.
	model = cp_model.CpModel()

	machines_count = len(processes[0].times)
	all_machines = range(machines_count)
	jobs_count = len(processes)
	all_jobs = range(jobs_count)

	jobs_data = get_jobs(processes)

	# maximal value of variables
	horizon = sum([sum(process.times) for process in processes])

	task_type = collections.namedtuple('task_type', 'start end interval')
	assigned_task_type = collections.namedtuple('assigned_task_type','start job index')

	# Create jobs.
	all_tasks = {}
	for job in range(len(processes)):
		for task_id, task in enumerate(jobs_data[job]):
			start_var = model.NewIntVar(0, horizon,'start_%i_%i' % (job, task_id))
			duration = task[1]
			end_var = model.NewIntVar(0, horizon, 'end_%i_%i' % (job, task_id))
			interval_var = model.NewIntervalVar(start_var, duration, end_var, 'interval_%i_%i' % (job, task_id))
			all_tasks[job, task_id] = task_type(start=start_var, end=end_var, interval=interval_var)

	# Create and add disjunctive constraints.
	for machine in range(len(processes[0].times)):
		intervals = []
		for job in range(len(processes)):
			for task_id, task in enumerate(jobs_data[job]):
				if task[0] == machine and job == 1: # only P times can't overlap in this problem
					intervals.append(all_tasks[job, task_id].interval)
		# add constraint
		model.AddNoOverlap(intervals)

	# Add precedence contraints.
	for job in all_jobs:
		for task_id in range(0, len(jobs_data[job]) - 1):
			model.Add(all_tasks[job, task_id + 1].start >= all_tasks[job, task_id].end)

	# Makespan objective.
	obj_var = model.NewIntVar(0, horizon, 'makespan')
	model.AddMaxEquality(obj_var, [all_tasks[(job, len(jobs_data[job]) - 1)].end for job in all_jobs])
	model.Minimize(obj_var)
	# Solve model.
	solver = cp_model.CpSolver()
	solver.StringParameters = "max_time_in_seconds:120.0"
	print("Solving")
	status = solver.Solve(model)

	return int(solver.ObjectiveValue())