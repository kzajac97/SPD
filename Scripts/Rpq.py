from __future__ import print_function

import collections

# Import Python wrapper for or-tools CP-SAT solver.
from ortools.sat.python import cp_model
from ortools.linear_solver import pywraplp

def get_jobs(processes):
	data = []
	for process in processes:
		process_data = []
		for i in range(len(process.times)):
			process_data.append((i,process.times[i]))
		data.append(process_data)

	return data

def MinimizeRpqMilp(processes):
	solver = pywraplp.Solver("Rpq_milp_optimizer",pywraplp.Solver.CBC_MIXED_INTEGER_PROGRAMMING)

	# sum of all task times 
	horizon = sum([sum(process.times) for process in processes])

	vars = []
	for i in range(len(processes)):
		var = solver.IntVar(0.0,horizon,'start_%i' % i)
		vars.append(var)

	for i in range(len(vars)):
		solver.Add(vars[i] + processes[i].times[1] >= processes[i].times[0])
		if i > 0:
			solver.Add(vars[i] >= vars[i-1])

	solver.Minimize(
		sum([process.times[0] for process in processes]) 
		+ sum(vars) 
		+ sum([process.times[2] for process in processes]))
	
	
	result_status = solver.Solve()
	print(solver.Objective().Value())
 
def MinimizeRpqJobshop(processes):
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

	print(solver.ObjectiveValue())