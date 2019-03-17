#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <typeinfo>
#include <iterator>
#include <exception>
#include <numeric>

#include "process.hh"
#include "utility.hh"

// computes time of tasks execution for given process order
// computes finish time of every task on every machine
int maxspan(std::vector<process> processes)
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
    return tasks.back().back(); 
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

auto getShortestTaskTime(std::vector<process> & processes, process task)
{
    int best_span = 1000000;
    unsigned int index;

    for(unsigned int i=0; i < processes.size() + 1; ++i)
    {
        auto current_processes = processes;
        current_processes.insert(current_processes.begin()+i,task);
        auto span = maxspan(current_processes);
        if(span < best_span)
        {
            best_span = span;
            index = i;
        }

        current_processes.clear();
        current_processes.shrink_to_fit();
    }

    return index;
}

std::vector<process> neh(std::vector<process> & processes)
{
    std::vector<process> result;
    auto ordered_processes = longestTotalTimeFirst(processes);
    result.push_back(ordered_processes[0]);

    for(unsigned int i=1; i < ordered_processes.size(); ++i)
    {
        auto x = getShortestTaskTime(result,ordered_processes[i]);
        result.insert(result.begin()+x,ordered_processes[i]);
    }

    return result;
}

int main(void)
{
    auto input = utility::readFile("data.txt");
    auto times = utility::createTimes(input);
    auto processes = utility::createProcesses(times);
    auto neh_result = neh(processes);
    std::cout << "neh result\n";
    for(auto x : neh_result) { std::cout << x.get_id() << " "; }
    std::cout << maxspan(neh_result) << "\n"; 
    std::cout << "\n";
} 