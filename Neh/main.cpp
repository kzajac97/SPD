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

#include "process.hh"
#include "utility.hh"

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

// returns permutation with shortest task at the beginning
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

int main(void)
{
    auto input = utility::readFile("data.txt");
    auto times = utility::createTimes(input);
    auto processes = utility::createProcesses(times);
    auto start = std::chrono::high_resolution_clock::now();
    auto neh_result = neh(processes);
    auto end = std::chrono::high_resolution_clock::now();
    auto start2 = std::chrono::high_resolution_clock::now();   
    auto result_acc = neh_accelerated(processes);
    auto end2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double,std::nano> diff = std::chrono::duration_cast<std::chrono::duration<double,std::nano>> (end-start);
    std::chrono::duration<double,std::nano> diff2 = std::chrono::duration_cast<std::chrono::duration<double,std::nano>> (end2-start2);

    std::cout << "Neh time: " << diff.count() << " \n";
    std::cout << "Accelerated time: " << diff2.count() << "\n"; 

    //std::cout << "Neh: " << maxspan(neh_result) << " Time: " << utility::getExecutionTime(neh,processes).count() << "\n";
    //std::cout << "Accelerated Neh: " << maxspan(result_acc) << " Time: " << utility::getExecutionTime(neh_accelerated,processes).count() << "\n";
    std::cout << "Neh: " << maxspan(neh_result) << " \n";
    std::cout << "Accelerated Neh: " << maxspan(result_acc) << "\n";

    for(auto x : neh_result) { std::cout << x.get_id() << " "; } std::cout << "\n";
    for(auto x : result_acc) { std::cout << x.get_id() << " "; } std::cout << "\n";
    
    std::cout << "\n";
} 