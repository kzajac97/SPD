from Process import *

def get_b_task(processes):
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

def get_a_task(processes):
    max_value = rpq_maxspan(processes)
    maxspan = [None] * len(processes)
    index = 0
    b_task, b_index = get_b_task(processes)

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

def get_c_task(processes):
    index = 0
    found = False

    _,a_index = get_a_task(processes)
    _,b_index = get_b_task(processes)

    for i in range(a_index,b_index):
        if processes[i].times[2] < processes[b_index].times[2]:
            index = i
            found = True

    if index == a_index or index == b_index:
        found = False

    return processes[index],index,found

def carlier(proceses):
    