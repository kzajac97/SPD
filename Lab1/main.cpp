#include <iostream>
#include <vector>
#include <algorithm>

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
    int executionTime;
    int previousTime = 0;

    for(int i=0; i < (int)processes.size(); ++i)  
    { 
        int currentTime = processes[i].getExecutionTime(); 
        for(int j=i; j > 0; --j)
        { 
            if(currentTime - processes[j-1].getExecutionTime() > 0)
                { previousTime += currentTime - processes[j-1].getExecutionTime(); } 
        } 
       
        executionTime += currentTime - previousTime;
    }

    return executionTime;
}

int main(void)
{
    auto input = readFile("data.txt");
    auto times = createTimes(input);
    auto processes = createProcesses(times);
    auto permutations = permutate(processes);

    for(auto permutation : permutations)
    {
        std::cout << getTimes(permutation) << "\n";
    }

} 