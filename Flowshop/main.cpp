#include <iostream>
#include <vector>
#include <algorithm>
#include <array>

#include "process.hh"
#include "utility.hh"

// computes permutations of given process vector
// <input> process vector 
// <returns> 2D vector containing process objects
// in every possible order (permutations)
std::vector<std::vector<process> > permutate(std::vector<process> processes)
{
    std::vector<std::vector<process> > result;

    do { result.push_back(processes); } 
    while (std::next_permutation(processes.begin(),processes.end()));

    return result;
}

int getTimes(std::vector<process> processes)
{
    // NOT IMPLEMENTED
    
    // int executionTime;
    // int delayTime = 0;
    // auto nMachines = processes[0].get_time().size();
    // auto nProcesses = processes.size();
    // std::vector<int> delayTimes;
    // delayTimes.resize(nMachines);

    // for(auto proc : processes)
    //     { executionTime += proc.getExecutionTime(); }

    // for(unsigned int mIter=1; mIter < nMachines; ++mIter)
    // {
    //     std::cout << "Process" << processes[i].get_id() << " ";
    //     for(unsigned int pIter=0; pIter < nProcesses; ++pIter)
    //     { 
    //         std::cout << processes[pIter].get_time()[mIter-1] << " ";
    //         int time2 = 0;
    //         if(pIter > 0)
    //             time2 += utility::relu(processes[pIter-1].get_time()[mIter]);
            
    //         delayTimes[mIter] += utility::relu(processes[pIter].get_time()[mIter-1] - time2);

    //     }
    //     std::cout << "\n";
    // }
    // for(auto x : delayTimes) { std::cout << x << " "; }
    // std::cout << "\n";
    // return executionTime + delayTime;
}

int main(void)
{
    auto input = utility::readFile("data.txt");
    auto times = utility::createTimes(input);
    auto processes = utility::createProcesses(times);
    auto permutations = permutate(processes);

    for(auto permutation : permutations)
    {
        std::cout << getTimes(permutation) << "\n";
    }
} 