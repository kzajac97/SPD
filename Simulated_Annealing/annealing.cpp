#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
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
#include <tuple>

#include <process.hpp>
#include <utility.hpp>  

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

unsigned int generateRandomIndex(unsigned int size)
{
    // generate random index solution
    std::mt19937 generator;
    std::uniform_int_distribution<> distribution(0,size-1);

    return distribution(generator);
}

std::tuple<unsigned int,unsigned int> generateRandomIndices(unsigned int size)
{
    // generate random index solution
    static std::mt19937 generator;
    std::uniform_int_distribution<> distribution(0,size-1);

    return std::make_tuple(distribution(generator),distribution(generator));
}

float acceptanceProbabilty(int cmax, int cmax_prime, int temperature)
{ 
    //return cmax > cmax_prime ? 1 : std::exp( (cmax-cmax_prime)/temperature );
    return cmax > cmax_prime ? 1 : 0;
}

int linear_cooling(int temperature)
{ 
    const float cooling_coeff = 0.75;
    return cooling_coeff * temperature;
}

int exponential_cooling(int temperature)
{
    return 0;
}

int logarithmic_cooling(int temperature)
{
    return 0;
}

int inverse_cooling(int temperature)
{
    return 0;
}

int inverse_linear_cooling(int temperature)
{
    return 0;
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

std::vector<process> simulate_annealing(std::vector<process> & processes, int start_temperature, int end_temperature, int iterations)
{
    int current_temperature = start_temperature;
    std::vector<process> current_processes;
    for(int i=0; i < iterations; ++i)
    {
        auto index = generateRandomIndices(processes.size());
        
        current_processes = processes;
        std::swap(current_processes[std::get<0>(index)],current_processes[std::get<1>(index)]);
        
        auto probability = acceptanceProbabilty(maxspan(processes),maxspan(current_processes),current_temperature);
        
        if(probability >= 1)
            { std::swap(processes[std::get<0>(index)],processes[std::get<1>(index)]); }

        current_temperature = linear_cooling(start_temperature);

        //if(current_temperature < end_temperature)
        //    { return current_processes; }

        current_processes.clear();
    }

    return processes;
}


int main(void)
{
    auto input = utility::readFile("Data/ta088.csv");
    auto times = utility::createTimes(input);
    auto processes = utility::createProcesses(times);
    auto neh_time = neh(processes);
    std::cout << "Neh: " << maxspan(neh_time) << "\n"; 
    auto result = simulate_annealing(neh_time,100,20,10000);
    std::cout << "Annealing: " << maxspan(result) << "\n";
}