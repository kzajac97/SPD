#include "scharge.hpp"

std::vector<process> sharge(std::vector<process> & processes)
{
    std::vector<process> result;
    std::vector<process> ready_tasks;

    for(unsigned int i=0; i < processes.size(); ++i)
    {
        //                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
    }
    
    return result;   
}

int rpq_maxspan(std::vector<process> & processes) 
{
    auto rp_processes = processes;
    std::vector<int> delivery_times;
    delivery_times.resize(rp_processes.size());

    for(auto & process : rp_processes)
        { process.pop_times(); }
    
    auto times = maxspan_vector(rp_processes);
    
    for(unsigned int i=0; i < delivery_times.size(); ++i)
        { delivery_times[i] = times.back()[i] + processes[i].get_time().back(); }

    times.push_back(delivery_times);

    return *std::max_element(times.back().begin(),times.back().end());
}