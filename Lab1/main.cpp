#include <iostream>
#include <vector>
#include <algorithm>

#include "process.hh"
#include "utility.hh"

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
    // not implemented 
    return 0;
} 