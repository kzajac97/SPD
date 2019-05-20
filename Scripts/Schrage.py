from Process import *
from copy import deepcopy

def schrage(processes):
    result = []
    schelduable_jobs = []
    copy = processes

    t = min([process.times[0] for process in processes])

    while len(processes) > 0 or len(schelduable_jobs) > 0:
        while len(processes) > 0 and min([process.times[0] for process in processes]) <= t:
            j = [process.times[0] for process in processes].index(min([process.times[0] for process in processes]))
            schelduable_jobs.append(processes[j])
            processes.pop(j)

        if len(schelduable_jobs) == 0:
            t = min([process.times[0] for process in processes])

        else:
            j = [process.times[2] for process in schelduable_jobs].index(max([process.times[2] for process in schelduable_jobs]))
            result.append(schelduable_jobs[j])
            t += schelduable_jobs[j].times[1]
            schelduable_jobs.pop(j)

    processes = copy
    return result

def schrage_pmtn(processes):
    #processes = deepcopy(processes)
    cmax = 0
    result = []
    schelduable_jobs = []
    jp = Process(0,[0,0,0])
    lp = Process(0,[0,0,0])
    t = 0

    while len(processes) > 0 or len(schelduable_jobs) > 0:
        while len(processes) > 0 and min([process.times[0] for process in processes]) <= t:
            jp = min(processes,key = lambda x : x.times[0])
            j = [process.times[0] for process in processes].index(min([process.times[0] for process in processes]))
            schelduable_jobs.append(jp)
            processes.pop(j)

            if jp.times[0] > lp.times[0]:
                lp.times[1] = t - jp.times[0]
                t = jp.times[0]

                if lp.times[1] > 0:
                    schelduable_jobs.append(lp)
            
        if len(schelduable_jobs) == 0:
            t = min([process.times[0] for process in processes])
        
        else:
            jp = max(schelduable_jobs,key = lambda x : x.times[2])
            j = [process.times[2] for process in schelduable_jobs].index(max([process.times[2] for process in schelduable_jobs]))
            schelduable_jobs.pop(j)

            lp = jp
            t += jp.times[1]

            if cmax < t + jp.times[2]:
                cmax = t + jp.times[2]

    return cmax