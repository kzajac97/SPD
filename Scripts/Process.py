# Class modeling process for RPQ flowshop problem
class Process:
    def __init__(self,init_id,init_times):
        self.id = init_id
        self.times = init_times

    def __str__(self):
        print("ID: ",self.id,"\n",self.times)
        
def read_data(filename):
    return [line.rstrip(' ') for line in open(filename)]

def create_processes(data):
    id = 1
    processes = []
    data = data[1:] # skip sizes in file
    # append process to list 
    for datastring in data:
        numeric_data = [ int(num) for num in list(filter(None,datastring.split(' ')))]
        processes.append(Process(id,numeric_data))
        id += 1

    return processes

def rpq_maxspan(processes):
    maxspan = [None] * len(processes)
    maxspan[0] = processes[0].times[0] + processes[0].times[1]

    for i in range(1,len(maxspan)):
        if processes[i].times[0] > maxspan[i - 1]:
            maxspan[i] = processes[i].times[0] + processes[i].times[1]
        else:
            maxspan[i] = processes[i].times[1] + maxspan[i-1]

    for j in range(len(maxspan)):
        maxspan[j] += processes[j].times[2]

    return max(maxspan)

def maxspan(processes):
    maxspan = [[None] * len(processes[0].times)] * len(processes)

    for i in range(len(processes)):
        for j in range(len(processes[i].times)):
            if i == 0 and j == 0:
                maxspan[i][j] = processes[i].times[j]
            elif i == 0 and j > 0:
                maxspan[i][j] = processes[i].times[j] + maxspan[i][j-1]
            elif i > 0 and j == 0:
                maxspan[i][j] = processes[i].times[j] + maxspan[i-1][j]
            else:
                if maxspan[i-1][j] > maxspan[i][j-1]:
                    maxspan[i][j] = processes[i].times[j] + maxspan[i-1][j]
                else:
                    maxspan[i][j] = processes[i].times[j] + maxspan[i][j-1]

    return maxspan[-1][-1] # return last element