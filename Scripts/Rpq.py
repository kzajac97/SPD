from __future__ import print_function

import collections

# Import Python wrapper for or-tools CP-SAT solver.

from ortools.sat.python import cp_model
from ortools.linear_solver import pywraplp

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

	all_tasks = {}
	for job in range(len(processes)):
		for task_id, task in enumerate(jobs_data[job]):
			start_var = model.NewIntVar(0, horizon,'start_%i_%i' % (job, task_id))
			duration = task[1]
			end_var = model.NewIntVar(0, horizon, 'end_%i_%i' % (job, task_id))
			interval_var = model.NewIntervalVar(start_var, duration, end_var, 'interval_%i_%i' % (job, task_id))
			all_tasks[job, task_id] = task_type(start=start_var, end=end_var, interval=interval_var)

	for machine in range(len(processes[0].times)):
		intervals = []
		for job in range(len(processes)):
			for task_id, task in enumerate(jobs_data[job]):
				if task[0] == machine and job == 1: # only P times can't overlap in this problem
					intervals.append(all_tasks[job, task_id].interval)

		model.AddNoOverlap(intervals)

	obj_var = model.NewIntVar(0, horizon, 'makespan')
	print(jobs_data[0])
	model.AddMaxEquality(obj_var, [all_tasks[(job, len(jobs_data[job]) - 1)].end + all_tasks(jobs_data[job]).end[2] for job in all_jobs])
	model.Minimize(obj_var)
	solver = cp_model.CpSolver()
	solver.Parameters = max_time_in_seconds = 120.0
	print("Solving")
	status = solver.Solve(model)

	print(solver.ObjectiveValue())