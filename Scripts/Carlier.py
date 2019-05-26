import copy

from Process import *
from Schrage import *

carlier_results = []
class Carlier:
    def __init__(self):
        self.carlier_results = []
        self.iter = 1

    def get_b_task(self,processes):
        max_value = rpq_maxspan(processes)
        maxspan = [None] * len(processes)
        index = 0

        maxspan[0] = processes[0].times[0] + processes[0].times[1]
        for i in range(1,len(processes)):
            if processes[i].times[0] > maxspan[i - 1]:
                maxspan[i] = processes[i].times[0] + processes[i].times[1]
            else:
                maxspan[i] = processes[i].times[1] + maxspan[i-1]

        for j in range(len(processes)):
            if maxspan[j] + processes[j].times[2] == max_value:
                index = j

        return processes[index],index

    def get_a_task(self,processes):
        max_value = rpq_maxspan(processes)
        maxspan = [None] * len(processes)
        index = 0
        b_task, b_index = self.get_b_task(processes)

        maxspan[0] = processes[0].times[0] + processes[0].times[1]
        for i in range(1,len(processes)):
            if processes[i].times[0] > maxspan[i - 1]:
                    maxspan[i] = processes[i].times[0] + processes[i].times[1]
            else:
                maxspan[i] = processes[i].times[1] + maxspan[i-1]

        j = b_index
        while j >= 1:
            if processes[j].times[0] < maxspan[j-1]:
                index = j

            j -= 1

        return processes[index],index

    def get_c_task(self,processes):
        index = 0
        found = False

        _,a_index = self.get_a_task(processes)
        _,b_index = self.get_b_task(processes)

        for i in range(a_index,b_index):
            if processes[i].times[2] < processes[b_index].times[2]:
                index = i
                found = True

        if index == a_index or index == b_index:
            found = False

        return processes[index],index,found

    def carlier(self,processes):
        self.carlier_results.append(rpq_maxspan(processes))
        if self.iter > 1000:
            return processes

        self.iter += 1

        u = rpq_maxspan(schrage(processes[:])) 
        upper_bound = rpq_maxspan(processes)
        lower_bound = 0
        optimal_result = processes
    
        if u < upper_bound:
            upper_bound = u
            processes = schrage(processes[:])
        
        a_task, a_index = self.get_a_task(processes)
        b_task, b_index = self.get_b_task(processes)
        c_task, c_index,c_found = self.get_c_task(processes)

        if c_found == False:
            self.carlier_results.append(rpq_maxspan(processes))
            return processes
            
        k_tasks = processes[c_index:b_index]

        rk = min([process.times[0] for process in k_tasks])
        qk = min([process.times[2] for process in k_tasks])
        pk = sum([process.times[1] for process in k_tasks])

        if rk+pk > c_task.times[0]:
            processes[c_index].times[0] = rk + pk

        r_restore = processes[c_index].times[0]

        processes_copy = copy.deepcopy(processes)
        lower_bound = schrage_pmtn(processes_copy)
        
        h_k_tasks = rk + pk + qk
        
        if rk < c_task.times[0]:
            rkc = rk
        else:
            rkc = c_task.times[0]

        if qk < c_task.times[2]:
            qkc = qk
        else:
            qkc = c_task.times[2]

        pkc = pk + c_task.times[1]

        h_kc_tasks = rkc + pkc + qkc

        if h_k_tasks > lower_bound:
            lower_bound = h_k_tasks
        if h_kc_tasks > lower_bound:
            lower_bound = h_kc_tasks

        if lower_bound < upper_bound:
            processes = self.carlier(processes)

        processes[c_index].times[0] = r_restore

        if qk + pk > processes[c_index].times[2]:
            processes[c_index].times[2] = qk + pk

        q_resotre = processes[c_index].times[2]

        processes_copy = copy.deepcopy(processes)
        lower_bound = schrage_pmtn(processes_copy)
        
        if rk < c_task.times[0]:
            rkc = rk
        else:
            rkc = c_task.times[0]

        if qk < c_task.times[2]:
            qkc = qk
        else:
            qkc = c_task.times[2]

        pkc = pk + c_task.times[1]

        if h_k_tasks > lower_bound:
            lower_bound = h_k_tasks

        if h_kc_tasks > lower_bound:
            lower_bound = h_kc_tasks

        if lower_bound < upper_bound:
            processes = self.carlier(processes)

        processes[c_index].times[2] = q_resotre
        
        self.carlier_results.append(rpq_maxspan(processes))
        return processes

    def run_carlier(self,processes):
        processes = self.carlier(processes)
        return min(self.carlier_results)