#include "scharge.hpp"

auto get_rpq_times(std::vector<process> processes)
{
    std::vector<int> rtimes, ptimes, qtimes;

    for(auto process : processes)
        { rtimes.push_back(process.get_time()[0]); }

    for(auto process : processes)
        { ptimes.push_back(process.get_time()[1]); }

    for(auto process : processes)
        { qtimes.push_back(process.get_time()[2]); }

    return std::make_tuple(rtimes,ptimes,qtimes);
}

std::vector<process> scharge(std::vector<process> processes)
{
    // vector of processes
    std::vector<process> result, schelduable_jobs;
    // get R P and Q times from tuple
    auto rpq_times = get_rpq_times(processes);
    std::vector<int> rtimes = std::get<0>(rpq_times);
    std::vector<int> ptimes = std::get<1>(rpq_times);
    std::vector<int> qtimes = std::get<2>(rpq_times);
    // helper vector holding indexes in array
    std::vector<unsigned int> indexes;
    int t = 0; // time variable to find schelduable tasks
    bool updated; // helper variable to update current time

    // loop until all processes are move to result vector
    while(!processes.empty() || !schelduable_jobs.empty())
    {
        updated = false;
        for(unsigned int i=0; i < rtimes.size(); i++)
        {
            // if preparation time is smaller than current T
            if(rtimes[i] <= t)
            {
                updated = true;           
                indexes.push_back(i);
            }      
        }
        
        // remove all scheldued processes using saved indices
        for(unsigned int i=0; i < indexes.size(); ++i)
        {
            schelduable_jobs.push_back(processes[indexes[i]]);
            processes.erase(processes.begin() + indexes[i]);
            rtimes.erase(rtimes.begin() + indexes[i]);
            // decrement all indices
            for(auto & index : indexes) 
                { index--; }
            // free memory
            processes.shrink_to_fit();
            rtimes.shrink_to_fit();
        }

        indexes.clear();
        indexes.shrink_to_fit();

        for(unsigned int j=0; j < schelduable_jobs.size(); ++j)
        {
            // get index of task with highet Q time
            unsigned int index = getMaxIndex(schelduable_jobs);
            result.push_back(schelduable_jobs[index]);
            // update time 
            t += schelduable_jobs[index].get_time()[1];
            updated = true;
            // remove from scheluded_jobs
            schelduable_jobs.erase(schelduable_jobs.begin() + index);           
            schelduable_jobs.shrink_to_fit();
        }

        if(!updated) { t++; } // if no process was updated increment t to find scheludable processes
    }

    for(auto x : result)
    { std::cout << x.get_id() -1 << " "; }
    return result;
}

unsigned int getMaxIndex(std::vector<process> processes)
{
    int max = 0;
    unsigned int index;
    
    for(unsigned int i=0; i < processes.size(); ++i)
    {
        if(processes[i].get_time().back() > max)
        {
            max = processes[i].get_time().back();
            index = i;
        }
    }

    return index;
}

int rpq_maxspan(std::vector<process> processes)
{
    auto rpq_times = get_rpq_times(processes);
    std::vector<int> rtimes = std::get<0>(rpq_times);
    std::vector<int> ptimes = std::get<1>(rpq_times);
    std::vector<int> qtimes = std::get<2>(rpq_times);
    std::vector<int> maxspan;
    maxspan.resize(rtimes.size());
    
    maxspan[0] = rtimes[0] + ptimes[0];
    for(unsigned int i=1; i < maxspan.size(); ++i)
    {
        if(rtimes[i] > maxspan[i-1])
            { maxspan[i] = rtimes[i] + ptimes[i]; }
        else
            { maxspan[i] = ptimes[i] + maxspan[i-1]; }
    }
    
    for(unsigned int j=0; j < qtimes.size(); ++j)
        { maxspan[j] += qtimes[j]; }
    
    return *std::max_element(maxspan.begin(), maxspan.end());
}
