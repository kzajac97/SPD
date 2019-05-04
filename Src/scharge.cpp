#include "scharge.hpp"
#include "heap.hpp"
#include "annealing.hpp"

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

std::vector<process> scharge_heap(std::vector<process> processes)
{
    // vector of processes
    std::vector<process> result, schelduable_jobs;
    // get R P and Q times from tuple
    auto rpq_times = get_rpq_times(processes);
    std::vector<int> rtimes = std::get<0>(rpq_times);
    std::vector<int> ptimes = std::get<1>(rpq_times);
    std::vector<int> qtimes = std::get<2>(rpq_times);

    auto rtimes_lambda = [](auto lhs, auto rhs) { return lhs < rhs; };
    auto processes_lambda = [](auto lhs, auto rhs) { return lhs.get_time()[0] < rhs.get_time()[0]; };
    
    buildHeap(rtimes,0,rtimes.size(),rtimes_lambda);
    buildHeap(processes,0,processes.size(),processes_lambda);

    int t = rtimes.front();

    while(!processes.empty() || !schelduable_jobs.empty())
    {
        while(!processes.empty() && *std::min_element(rtimes.begin(),rtimes.end()) /*rtimes[0]*/ <= t)
        {
            unsigned int j = getMinIndex(processes);
            schelduable_jobs.push_back(processes[j]);
            processes.erase(processes.begin() + j);
            rtimes.erase(rtimes.begin() + j);
            processes.shrink_to_fit();
            rtimes.shrink_to_fit();
            heapify(processes,0,processes.size(),processes.size(),processes_lambda);
            heapify(rtimes,0,rtimes.size(),rtimes.size(),rtimes_lambda);
        }

        if(schelduable_jobs.empty())
        { 
            t = *std::min_element(rtimes.begin(),rtimes.end());
            heapify(rtimes,0,rtimes.size(),rtimes.size(),rtimes_lambda);
        }

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

std::vector<process> scharge_heap_fast(std::vector<process> processes)
{
    // vector of processes
    std::vector<process> result, schelduable_jobs;
    // get R P and Q times from tuple
    auto rpq_times = get_rpq_times(processes);
    std::vector<int> rtimes = std::get<0>(rpq_times);
    std::vector<int> ptimes = std::get<1>(rpq_times);
    std::vector<int> qtimes = std::get<2>(rpq_times);

    auto rtimes_lambda = [](auto lhs, auto rhs) { return lhs < rhs; };
    auto processes_lambda = [](auto lhs, auto rhs) { return lhs.get_time()[0] < rhs.get_time()[0]; };
    
    buildHeap(rtimes,0,rtimes.size(),rtimes_lambda);
    buildHeap(processes,0,processes.size(),processes_lambda);

    int t = rtimes.front();

    while(!processes.empty() || !schelduable_jobs.empty())
    {
        while(!processes.empty() && rtimes.front() <= t)
        {
            schelduable_jobs.push_back(processes.front());
            processes.erase(processes.begin());
            rtimes.erase(rtimes.begin());
            processes.shrink_to_fit();
            rtimes.shrink_to_fit();
            heapify(processes,0,processes.size(),processes.size(),processes_lambda);
            heapify(rtimes,0,rtimes.size(),rtimes.size(),rtimes_lambda);
        }

        if(schelduable_jobs.empty())
        { 
            t = rtimes.front();
            heapify(rtimes,0,rtimes.size(),rtimes.size(),rtimes_lambda);
        }

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
    process lp(0,{0,0,0});

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

int schrage_pmtn_heap(std::vector<process> processes)
{
    int cmax = 0;
    std::vector<process> result, schelduable_jobs;
    int t = 0;
    unsigned int j;
    process jp;
    process lp(0,{0,0,0});

    auto rpq_times = get_rpq_times(processes);
    std::vector<int> rtimes = std::get<0>(rpq_times);
    std::vector<int> ptimes = std::get<1>(rpq_times);
    std::vector<int> qtimes = std::get<2>(rpq_times);

    auto processes_lambda = [](auto lhs, auto rhs) { return lhs.get_time()[0] < rhs.get_time()[0]; }; 
    buildHeap(processes,0,processes.size(),processes_lambda);

    while(!processes.empty() || !schelduable_jobs.empty())
    {
        while(!processes.empty() && getMin(processes) <= t)
        {
            jp = getRmin(processes);
            j = getMinIndex(processes);
            schelduable_jobs.push_back(jp);
            processes.erase(processes.begin() + j);
            processes.shrink_to_fit();
            heapify(processes,0,processes.size(),processes.size(),processes_lambda);

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