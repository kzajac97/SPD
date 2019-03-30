#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <typeinfo>
#include <iterator>
#include <exception>
#include <numeric>

#include <process.hh>
#include <utility.hh>

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
    for(unsigned int i=0; i < virtual_times.size(); ++i)
    {
        for(unsigned int j=0; j < virtual_times.size(); ++j)
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

int main(void)
{
    auto input = utility::readFile("data.txt");
    auto times = utility::createTimes(input);
    auto processes = utility::createProcesses(times);
    // auto permutations = getPermutations(processes);

    // std::cout << "Spans for all permutations\n";
    // for(auto permutation : permutations)
    //     { std::cout << maxspan(permutation) << " "; }
    // std::cout << "\n";

    // std::vector<std::vector<int> > machine2times = {{32,42},
    //                                                 {47,15},
    //                                                 {22,50},
    //                                                 {58,40},
    //                                                 {31,28}};

    
    // std::vector<std::vector<int> > machine3times = {{32,42,10},
    //                                                 {47,15,10},
    //                                                 {22,50,15},
    //                                                 {58,40,5},
    //                                                 {31,28,20}};

    // auto processes2johnson = utility::createProcesses(machine2times);
    // auto johnson2 = johnson2machine(processes2johnson);
    // std::cout << "2 machine johnson solution\n";
    // for(auto proc2 : johnson2)
    //     { std::cout << proc2.get_id() << " "; }
    // std::cout << maxspan(johnson2) << "\n";

    // auto processes3johnson = utility::createProcesses(machine3times);
    // auto johnson3 = johnson3machine(processes3johnson);
    // std::cout << "3 machine johnson solution\n";
    // for(auto proc3 : johnson3)
    //     { std::cout << proc3.get_id() << " "; }
    // std::cout << maxspan(johnson3) << "\n";

    auto x = shortestTaskFirst2(processes);
    for(auto it : x) { std::cout << it.get_id() << " "; }
    std::cout << "\n";
} 