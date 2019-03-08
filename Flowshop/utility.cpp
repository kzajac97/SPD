#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <functional>

#include "utility.hh"

using namespace utility;

std::vector<numeric_t> utility::readFile(const std::string & filename)
{
    std::ifstream inputFile(filename);
    std::vector<numeric_t> outputVec;
    std::istream_iterator<numeric_t> input(inputFile); // copy file contents with auto iterator
    std::copy(input, std::istream_iterator<numeric_t>(),std::back_inserter(outputVec));

    return outputVec;
}

std::vector<std::vector<numeric_t> > utility::createTimes(const std::vector<numeric_t> & input)
{
    unsigned int nMachines = input[0]; //numerc of machines in input, int could be other numeric 
    unsigned int nProcesses = input[1]; //numer of processes for each machine
    const unsigned int offset = 2; // offset from input vector begining to where actual times data is
    unsigned int index = offset; //index for input vector data, initially 2
    std::vector<std::vector<numeric_t> > times; // output 2D vector of times  
    
    // resize every vector in times
    times.resize(nMachines);
    // resize vectors inside 
    for(auto & it : times) 
        { it.resize(nProcesses); }
    
    // assign values using indexes splitting input vector
    // into 2D vector of sizes given in input data
    for(unsigned int i=0; i < nMachines; ++i)
    {
        for(unsigned j=0; j < nProcesses; ++j)
        { 
            times[i][j] = input[index]; 
            index++; 
        }
    }

    return times;
}

std::vector<process> utility::createProcesses(std::vector<std::vector<int> > times)
{
    unsigned int index = 1;
    std::vector<process> result;
    for(auto it : times)
    { 
        result.push_back(process(index,it)); 
        index++;
    }

    return result;
}

std::vector<std::vector<int> > utility::getTimespan(std::vector<process> processes)
{
    std::vector<std::vector<int> > timespan;
    // resize timespan to match number of processes and machines
    timespan.resize(processes[0].get_time().size());
    for(auto & it : timespan)
        { it.resize(processes.size()); }

    for(unsigned int i=0; i < timespan.size(); ++i)
    {
        for(unsigned int j=0; j < timespan[0].size(); ++j)
            { timespan[i][j] = processes[j].get_time()[i]; }
    }

    return timespan;
}
