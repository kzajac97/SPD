#include <algorithm>
#include <array>
#include <typeinfo>
#include <iterator>
#include <exception>
#include <numeric>
#include <thread>
#include <future>
#include <functional>
#include <utility>
#include <limits>
#include <chrono>

#include "neh.hpp"
#include "flowshop.hpp"

// returns permutation with longest total time of task on every machine
std::vector<process> longestTotalTimeFirst(std::vector<process> & processes)
{
    // sort processes vector with lamdba comparator
    std::sort(processes.begin(),processes.end(),[](auto & lhs, auto & rhs)
    {
        auto left_times = lhs.get_time(); // get vectors of times of current processes
        auto right_times = rhs.get_time();
        // compare total time of process
        return std::accumulate(left_times.begin(),left_times.end(),0) > std::accumulate(right_times.begin(),right_times.end(),0);
    });

    return processes;
}

// uses stable_sort (merge) instead of quick sort
std::vector<process> longestTotalTimeFirstStable(std::vector<process> & processes)
{
    std::stable_sort(processes.begin(),processes.end(),[](auto lhs, auto rhs)
    {
        auto left_times = lhs.get_time(); // get vectors of times of current processes
        auto right_times = rhs.get_time();
        // compare total time of process
        return std::accumulate(left_times.begin(),left_times.end(),0) > std::accumulate(right_times.begin(),right_times.end(),0);
    });

    return processes;
}

// finds index which gives smallest maxspan for current task insertion
unsigned int getShortestTaskTime(std::vector<process> & processes, process task)
{
    int best_span = std::numeric_limits<int>::max();
    unsigned int index; // index of current task best placment

    // insert element on each position in loop
    // and save index giving smallest total time
    for(unsigned int i=0; i <= processes.size(); ++i) // loop one element further than processes size
    {
        auto current_processes = processes;
        current_processes.insert(current_processes.begin()+i,task); 
        auto span = maxspan(current_processes);
        // save best span and it's index
        if(span < best_span)
            { best_span = span; index = i; }

        current_processes.clear(); // clear values
        current_processes.shrink_to_fit(); // deallocate memory
    }

    return index;
}

// finds index which gives smallest maxspan for current task insertion
// reuses computation if there is a lot of processes
auto getShortestTaskTimeAccelerated(std::vector<process> & processes, process task)
{
    int best_span = std::numeric_limits<int>::max();
    unsigned int index;

    if(processes.size() < 4) // for small number of processes acceleration is not necessery
        return getShortestTaskTime(processes, task);
    
    else
    {
        // get max span with current task at the back
        auto task_order = processes; 
        task_order.push_back(task);
        auto current_tasks = maxspan_vector(task_order);
        
        for(unsigned int i=0; i <= processes.size(); ++i)
        {
            auto current_processes = processes;
            current_processes.insert(current_processes.begin()+i,task); 
            // reuse timespan vector
            auto span = maxspan_accelerated(current_processes,i,current_tasks);
            // save best span and it's index
            if(span < best_span)
                { best_span = span; index = i; }

            current_processes.clear(); // clea values
            current_processes.shrink_to_fit(); // deallocate memory
        }
    }

    return index;
}

auto getMostExpansiveTask(std::vector<process> & processes)
{
    int best_span = std::numeric_limits<int>::max();
    unsigned int index;
    process task;

    for(unsigned int i=0; i <= processes.size(); ++i) // loop one element further than processes size
    {
        auto current_processes = processes;
        current_processes.erase(current_processes.begin()+i); 
        auto span = maxspan(current_processes);
        // save best span and it's index
        if(span < best_span)
            { best_span = span; index = i; task = current_processes[i]; }

        current_processes.clear(); // clear values
        current_processes.shrink_to_fit(); // deallocate memory
    }

    return std::make_tuple(index,task);
}

// implementation of neh schelduing algorithm
std::vector<process> neh(std::vector<process> & processes)
{
    std::vector<process> result;
    auto ordered_processes = longestTotalTimeFirstStable(processes);
    result.push_back(ordered_processes[0]);

    for(unsigned int i=1; i < ordered_processes.size(); ++i)
    {
        auto task_time = getShortestTaskTime(result,ordered_processes[i]);
        result.insert(result.begin()+task_time,ordered_processes[i]);
    }

    return result;
}

// neh with reused computations 
// Acceleration is about 20% faster than normal neh
std::vector<process> neh_accelerated(std::vector<process> & processes)
{
    std::vector<process> result;
    auto ordered_processes = longestTotalTimeFirstStable(processes);
    result.push_back(ordered_processes[0]);

    for(unsigned int i=1; i < ordered_processes.size(); ++i)
    {
        auto task_time = getShortestTaskTimeAccelerated(result,ordered_processes[i]);
        result.insert(result.begin()+task_time,ordered_processes[i]);
    }

    return result;
}

std::vector<process> neh_modified(std::vector<process> & processes)
{
    std::vector<process> result;
    auto ordered_processes = longestTotalTimeFirstStable(processes);
    result.push_back(ordered_processes[0]);

    for(unsigned int i=1; i < ordered_processes.size(); ++i)
    {
        auto task_index = getShortestTaskTimeAccelerated(result,ordered_processes[i]);
        result.insert(result.begin()+task_index,ordered_processes[i]);
        if(i > 2)
        {
            auto task_x = getMostExpansiveTask(result);
            result.erase(result.begin()+ std::get<0>(task_x) );
            auto task_x_index = getShortestTaskTime(result,std::get<1>(task_x));
            result.insert(result.begin()+task_x_index,std::get<1>(task_x));
        }
    }

    return result;
}