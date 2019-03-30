#include <iostream>
#include <vector>
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

#include <process.hh>
#include <utility.hh>

// computes time of tasks execution for given process order
// computes finish time of every task on every machine
// <returns> a vector of vectors holding every task finish time
auto maxspan_vector(std::vector<process> processes)
{
    auto timespan = utility::getTimespan(processes);
    std::vector<std::vector<int> > tasks; // finish times
    
    tasks.resize(processes[0].get_time().size());
    for(auto & it : tasks)
        { it.resize(processes.size()); }

    for(unsigned int i=0; i < timespan.size(); ++i)
    {
        for(unsigned int j=0; j < timespan[i].size(); ++j)
        {
            if(i == 0 && j == 0) // first machine first process
                { tasks[i][j] = timespan[i][j]; }
            else if(i == 0 && j > 0) // first machine all processes
                { tasks[i][j] = timespan[i][j] + tasks[i][j-1]; }
            else if(i > 0 && j == 0) // first process all machines
                { tasks[i][j] = timespan[i][j] + tasks[i-1][j]; }
            else // all machines all processes
            {
                // machine waits for previous machine to complete task 
                // and for itself to complete previous task
                // delay is the longer of this two times
                tasks[i][j] = (tasks[i-1][j] > tasks[i][j-1]) ?  
                    timespan[i][j] + tasks[i-1][j] : 
                    tasks[i][j] = timespan[i][j] + tasks[i][j-1]; // long ternary operator
            }          
        }
    }
    // return finish time of last task on last machine
    return tasks; 
}

// returns value of maxspan
int maxspan(std::vector<process> processes)
{
    return maxspan_vector(processes).back().back(); 
}

// accelerated vesrion of maxspan using times from previoulsy comupted task times
int maxspan_accelerated(std::vector<process> processes, unsigned int start_index, std::vector<std::vector<int> > current_tasks)
{
    auto timespan = utility::getTimespan(processes);
    auto tasks = current_tasks; // get task times from previous iterations
    
    for(unsigned int i=0; i < timespan.size(); ++i)
    {
        // start loop in place where new task was inserted
        for(unsigned int j=start_index; j < timespan[i].size(); ++j)
        {
            if(i == 0 && j == 0) // first machine first process
                { tasks[i][j] = timespan[i][j]; }
            else if(i == 0 && j > 0) // first machine all processes
                { tasks[i][j] = timespan[i][j] + tasks[i][j-1]; }
            else if(i > 0 && j == 0) // first process all machines
                { tasks[i][j] = timespan[i][j] + tasks[i-1][j]; }
            else // all machines all processes
            {
                // machine waits for previous machine to complete task 
                // and for itself to complete previous task
                // delay is the longer of this two times
                tasks[i][j] = (tasks[i-1][j] > tasks[i][j-1]) ?  
                    timespan[i][j] + tasks[i-1][j] : 
                    tasks[i][j] = timespan[i][j] + tasks[i][j-1]; // long ternary operator
            }       
        }
    }
    // return finish time of last task on last machine
    return tasks.back().back(); 
}

// computes permutations of given process vector
// <input> process vector 
// <returns> 2D vector containing process objects
// in every possible order (permutations)
std::vector<std::vector<process> > getPermutations(std::vector<process> processes)
{
    std::vector<std::vector<process> > result;

    do { result.push_back(processes); } 
    while (std::next_permutation(processes.begin(),processes.end()));

    return result;
}


std::vector<process> shortestTaskFirst(std::vector<process> & processes)
{
    // sort processes vector with lamdba comparator
    std::sort(processes.begin(),processes.end(),[](auto & lhs, auto & rhs)
    {
        auto left_times = lhs.get_time(); // get vectors of times of current processes
        auto right_times = rhs.get_time();
        // compare smallest time on every machine of current two processes
        return *std::min_element(left_times.begin(),left_times.end()) < *std::min_element(right_times.begin(),right_times.end());
    });

    return processes;
}

std::vector<process> johnson2machine(std::vector<process> processes)
{
    // check condition for 2 machine problem
    for(auto process : processes)
    { 
        if(process.get_time().size() != 2)
            { throw std::invalid_argument("Invalid argument"); }
    }

    std::vector<process> result;
    std::vector<process> result_back;
    auto sorted_processes = shortestTaskFirst(processes); // get processes on sorted vector
    
    for(auto process : sorted_processes)
    {
        if(process.get_time().front() < process.get_time().back())
            { result.push_back(process); }
        else
            { result_back.push_back(process); }
    }

    // insert processes inserted on the back with reverse iterator
    for(auto iter = result_back.rbegin(); iter != result_back.rend(); ++iter)
        { result.push_back(*iter); }

    return result;
}

std::vector<process> johnson3machine(std::vector<process> processes)
{
    std::vector<process> result;

    //check condition for 3 machine problem
    for(auto process : processes)
    { 
       if(process.get_time().size() != 3)
           { throw std::invalid_argument("Invaild argument"); }
    }

    // virtual times 2D vector
    std::vector<std::vector<int> > virtual_times(processes.size(),std::vector<int>(2));
    // create 2 virtual machines in which times in virtual machine 1 are summed times
    // from machine 1 and 2 and in virtual machine 2 times from machine 2 and 3
    for(unsigned int i=0; i <= virtual_times.size(); ++i)
    {
        for(unsigned int j=0; j <= virtual_times.size(); ++j)
        { virtual_times[i][j] = processes[i].get_time()[j] + processes[i].get_time()[j+1]; }
    }

    auto virtual_processes = utility::createProcesses(virtual_times);
    // use johnson algorithm for 2 machine problem
    auto virtual_order = johnson2machine(virtual_processes);
    // sort original processes in returned order 
    for(auto virtual_process : virtual_order)
    {
        for(auto process : processes)
        {
            if(process.get_id() == virtual_process.get_id())
                { result.push_back(process); }
        }
    }

    return result;
}

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
auto getShortestTaskTime(std::vector<process> & processes, process task)
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

int main(void)
{
    auto input = utility::readFile("Data/ta000.txt");

    for(auto x : input) { std::cout << x << "\n"; }

    auto times = utility::createTimes(input);
    auto processes = utility::createProcesses(times);
    
    auto neh_time = neh(processes);
    auto neh_mod = neh_modified(processes);
    
    std::cout << "Neh: " << maxspan(neh_time) << "\n";
    std::cout << "Modified: " << maxspan(neh_mod) << "\n";

    std::cout << "\n";
} 