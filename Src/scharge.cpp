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

    int t = *std::min_element(rtimes.begin(),rtimes.end()); // time variable to find schelduable tasks
    
    while(!processes.empty() || !schelduable_jobs.empty())
    {
        while(!processes.empty() && *std::min_element(rtimes.begin(),rtimes.end()) <= t)
        {
            unsigned int j = getMinIndex(processes);
            schelduable_jobs.push_back(processes[j]);
            processes.erase(processes.begin() + j);
            rtimes.erase(rtimes.begin() + j);
            processes.shrink_to_fit();
            rtimes.shrink_to_fit();
        }

        if(schelduable_jobs.empty())
            { t = *std::min_element(rtimes.begin(),rtimes.end()); }

        else
        {
            unsigned int j = getMaxIndex(schelduable_jobs);
            result.push_back(schelduable_jobs[j]);
            t += schelduable_jobs[j].get_time()[1];
            schelduable_jobs.erase(schelduable_jobs.begin() + j);
            schelduable_jobs.shrink_to_fit();
        }
    }

    return result;
}

int schrage_pmtn(std::vector<process> processes)
{
    int cmax = 0;
    std::vector<process> result, schelduable_jobs;
    int t = 0;
    unsigned int j;
    process jp;
    process lp(0,{1000,1000,1000});

    auto rpq_times = get_rpq_times(processes);
    std::vector<int> rtimes = std::get<0>(rpq_times);
    std::vector<int> ptimes = std::get<1>(rpq_times);
    std::vector<int> qtimes = std::get<2>(rpq_times);

    while(!processes.empty() || !schelduable_jobs.empty())
    {
        while(!processes.empty() && getMin(processes) <= t)
        {
            jp = getRmin(processes);
            j = getMinIndex(processes);
            schelduable_jobs.push_back(jp);
            processes.erase(processes.begin() + j);
            processes.shrink_to_fit();

            if(jp.get_time()[0] > lp.get_time()[0])
            {
                lp.set_time(t - jp.get_time()[0], 1);
                t = jp.get_time()[0];

                if(lp.get_time()[1] > 0)
                    { schelduable_jobs.push_back(lp); } 
            }
        }

        if(schelduable_jobs.empty())
            { t = getMin(processes); }
        else
        {
            jp = getQmax(schelduable_jobs);
            j = getMaxIndex(schelduable_jobs);
            schelduable_jobs.erase(schelduable_jobs.begin() + j);
            schelduable_jobs.shrink_to_fit();

            lp = jp;
            t += jp.get_time()[1];
            if(cmax < t + jp.get_time().back())
                { cmax = t + jp.get_time().back(); }
        }
    }
    
    return cmax;
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

unsigned int getMinIndex(std::vector<process> processes)
{
    int min = std::numeric_limits<int>::max();
    unsigned int index;
    
    for(unsigned int i=0; i < processes.size(); ++i)
    {
        if(processes[i].get_time()[0] < min)
        {
            min = processes[i].get_time()[0];
            index = i;
        }
    }

    return index;
}

int getMin(std::vector<process> processes)
{
    int min = std::numeric_limits<int>::max();

    for(unsigned int i=0; i < processes.size(); ++i)
    {
        if(processes[i].get_time()[0] < min)
            { min = processes[i].get_time()[0]; }
    }

    return min;
}

process getRmin(std::vector<process> processes)
{
    int min = std::numeric_limits<int>::max();
    process result;

    for(auto process : processes)
    {
        if(process.get_time()[0] < min)
        {
            result = process;
            min = process.get_time()[0];
        }
    }
    
    return result;
}

process getQmax(std::vector<process> processes)
{
    int max = 0;
    process result;

    for(auto process : processes)
    {
        if(process.get_time().back() > max)
        {
            result = process;
            max = process.get_time().back();
        }
    }
    
    return result;
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
