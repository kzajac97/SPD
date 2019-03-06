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

int makespan(std::vector<process> processes)
{
    int executionTime;
    int delayTime;
    auto timespan = utility::getTimespan(processes);

    for(auto process : processes)
        { executionTime += process.getExecutionTime(); }

    for(unsigned int mIter=0; mIter < timespan.size(); ++mIter)
    {
        for(unsigned int pIter=0; pIter < timespan[0].size(); ++pIter)
        {
            if(mIter == 0) { } // first machine 
            else if(pIter == 0) // first process
                { delayTime += utility::getColSum(timespan,0,mIter-1,0); }
            else // delay between processes
                { delayTime += utility::relu(timespan[mIter-1][pIter] - timespan[mIter][pIter-1]); }
            
        }
    } 

    return executionTime + delayTime;
}

int main(void)
{
    auto input = utility::readFile("data.txt");
    auto times = utility::createTimes(input);
    auto processes = utility::createProcesses(times);
    //auto permutations = permutate(processes);
    auto x = makespan(processes);
    std::cout << x << "\n";
} 