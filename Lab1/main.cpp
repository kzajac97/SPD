#include <iostream>
#include <vector>
#include <algorithm>

#include "process.hh"

std::vector<process> create_processes(std::vector<std::vector<int> > times)
{
    unsigned int index = 1;
    std::vector<process> result;
    for(auto it : times)
        { 
            result.push_back( process(index,it) ); 
            index++;
        }

    return result;
}

std::vector<std::vector<process> > permutate(std::vector<process> processes)
{
    std::vector<std::vector<process> > result;

    do
    {
        result.push_back(processes);
    } while (std::next_permutation(processes.begin(),processes.end()) );

    return result;
}


int main(void)
{
    std::vector<std::vector<int> > times = {{1,2,3,4},
                                {2,3,4,5}};

    auto processes = create_processes(times);
}