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

std::vector<std::vector<numeric_t> > utility::generateRandomTimes(unsigned int nMachines, unsigned int nProcesses, int minValue, int maxValue)
{
    // times can't be negative 
    if(minValue < 0 || maxValue < 0)
        { throw std::invalid_argument("Invalid arguments"); }

    // returned vector
    std::vector<std::vector<numeric_t> > random_times(nProcesses,std::vector<numeric_t>(nMachines));
    // create random distributions
    std::random_device rnd;
    std::mt19937 mersenne_engine(rnd());
    std::uniform_int_distribution<> dist(minValue,maxValue);
    
    // create random vectors in lambda function capturing dist and engine variables
    for(auto & iter : random_times)
        { std::generate(iter.begin(),iter.end(),[&dist,&mersenne_engine]() mutable 
            { return dist(mersenne_engine); }); }

    return random_times;
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

// graph_t utility::getGraphTimespan(std::vector<std::vector<int> > timespan)
// {
//     graph_t graph_span(timespan.size()*timespan[0].size()); // create empty graph
//     std::vector<int> times;
//     auto shift = timespan[0].size();
//     unsigned int time_iter = 0;

//     for(unsigned int i=0; i < timespan.size(); ++i)
//     {
//         for(unsigned int j=0; j < timespan[0].size(); ++j)
//         {
//             times.push_back(timespan[i][j]);
//             // add edge from process to next process on every machine
//             // don't add if it's last process for current machine
//             if(j+1 < timespan[0].size())
//                 { boost::add_edge((i*shift) + j, 
//                                   (i*shift) + j + 1,                                                                                                             
//                                     graph_span); }
//             // add edge between process and the same process on next machine
//             // don't add for last machine
//             if(i+1 < timespan.size())
//                 { boost::add_edge((i*shift) + j, 
//                                   (i*shift) + j + shift,
//                                     graph_span); }
//         }
//     }

//     for (auto vertex_iterator : boost::make_iterator_range(vertices(graph_span)))
//     {
//         graph_span[vertex_iterator].task_time = times[time_iter];
//         time_iter++;
//     }

//     return graph_span;
// }