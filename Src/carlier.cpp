#include <numeric>

#include "carlier.hpp"

std::vector<process> carlier(std::vector<process> & processes)
{
    int u = rpq_maxspan(scharge(processes));
    auto result = processes;
    int upper_bound = rpq_maxspan(processes);
    int lower_bound = 0;
    std::vector<process> optimal_result = processes;

    if(u < upper_bound)
    { 
        upper_bound = u;
        result = scharge_heap(processes); 
    }
    // get reference tasks
    auto a_task = get_a_task(result);
    auto b_task = get_b_task(result);
    auto c_task = get_c_task(result);

    if(!std::get<2>(c_task)) // if C task is not found
    { 
        if(rpq_maxspan(result) < rpq_maxspan(optimal_result))
            { optimal_result = result; }
    }
    // create K vector
    std::vector<process> k_tasks(result.begin() + std::get<1>(c_task),result.begin() + std::get<1>(b_task));
    // Get R P and Q times for K vector and extract tuple to vectors 
    auto k_rpq_times = get_rpq_times(k_tasks);
    std::vector<int> krtimes = std::get<0>(k_rpq_times);
    std::vector<int> kptimes = std::get<1>(k_rpq_times);
    std::vector<int> kqtimes = std::get<2>(k_rpq_times);
    // get R and Q times as minimum and P time as sum of all elements
    int rk = *std::min_element(krtimes.begin(),krtimes.end()); 
    int qk = *std::min_element(kqtimes.begin(),kqtimes.end());
    int pk = std::accumulate(kptimes.begin(),kptimes.end(),0);

    if(rk + pk >  result[std::get<1>(c_task)].get_time()[0])
        { result[std::get<1>(c_task)].set_time(rk + pk,0); }

    lower_bound = schrage_pmtn(result);

    // Get R P Q times for K vector with C task inserted    
    int h_k_tasks = rk + pk + qk;
    int rkc = rk < std::get<0>(c_task).get_time().front() ? rk : std::get<0>(c_task).get_time().front();
    int qkc = qk < std::get<0>(c_task).get_time().back() ? rk : std::get<0>(c_task).get_time().back();
    int pkc = pk + std::get<0>(c_task).get_time()[1];
   
    int h_kc_tasks = rkc + qkc + pkc;

    // Update bounds
    if(h_k_tasks > lower_bound)
        { lower_bound = h_k_tasks; }
    
    if(h_kc_tasks > lower_bound)
        { lower_bound = h_kc_tasks; }

    ////
    if(lower_bound < upper_bound)
        { result = carlier(result); }
    
    result[std::get<1>(c_task)].set_time(std::get<0>(c_task).get_time()[0],0);

    if(qk + pk > result[std::get<1>(c_task)].get_time().back())
        { result[std::get<1>(c_task)].set_time(qk + pk ,0); }

    lower_bound = schrage_pmtn(result);
    // update R P Q times for K vector
    rkc = rk < std::get<0>(c_task).get_time().front() ? rk : std::get<0>(c_task).get_time().front();
    qkc = qk < std::get<0>(c_task).get_time().back() ? rk : std::get<0>(c_task).get_time().back();
    pkc = pk + std::get<0>(c_task).get_time()[1];

    h_kc_tasks = rkc + qkc + pkc;

    if(h_k_tasks > lower_bound)
        { lower_bound = h_k_tasks; }
    
    if(h_kc_tasks > lower_bound)
        { lower_bound = h_kc_tasks; }

    if(lower_bound < upper_bound)
        { result = carlier(result); }
    
    result[std::get<1>(c_task)].set_time(std::get<0>(c_task).get_time().back(),2);

    // clear memory
    k_tasks.clear();
    k_tasks.shrink_to_fit();

    return optimal_result;
}

std::tuple<process,unsigned> get_b_task(std::vector<process> processes)
{
    int maxspan_value = rpq_maxspan(processes);
    unsigned int index = processes.size() - 1;
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
    { 
        if(maxspan[j] + qtimes[j] == maxspan_value)
            { index = j; } 
    }

    return std::make_tuple(processes[index],index);
}

std::tuple<process,unsigned> get_a_task(std::vector<process> processes)
{
    unsigned int index = 0;
    auto rpq_times = get_rpq_times(processes);
    std::vector<int> rtimes = std::get<0>(rpq_times);
    std::vector<int> ptimes = std::get<1>(rpq_times);
    std::vector<int> maxspan;
    maxspan.resize(rtimes.size());
    auto b_task = get_b_task(processes);

    maxspan[0] = rtimes[0] + ptimes[0];
    for(unsigned int i=1; i < maxspan.size(); ++i)
    {
        if(rtimes[i] > maxspan[i-1])
            { maxspan[i] = rtimes[i] + ptimes[i]; }
        else
            { maxspan[i] = ptimes[i] + maxspan[i-1]; }
    }

    for(unsigned int j = std::get<1>(b_task); j >= 1; --j)
    {
        if(rtimes[j] < maxspan[j - 1])
            { index = j; }
    }

    return std::make_tuple(processes[index],index);
}

std::tuple<process,unsigned,bool> get_c_task(std::vector<process> processes)
{
    unsigned int index = 0;
    auto rpq_times = get_rpq_times(processes);
    std::vector<int> qtimes = std::get<2>(rpq_times);
    bool found = false;
    auto a_task = get_a_task(processes);
    auto b_task = get_b_task(processes);

    for(unsigned int i = std::get<1>(a_task); i < std::get<1>(b_task); ++i)
    {
        if(qtimes[i] < qtimes[std::get<1>(b_task)])
        { 
            index = i; 
            found = true;
        }
    }

    if(index == std::get<1>(a_task) || index == std::get<1>(b_task))
        { found = false; }
    
    return std::make_tuple(processes[index],index,found);
}