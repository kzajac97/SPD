#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>

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
    static std::mt19937 generator;
    std::uniform_int_distribution<> distribution(0,size-1);

    return distribution(generator);
}

float acceptanceProbabilty(int cmax, int cmax_prime, int temperature)
{ return cmax > cmax_prime ? 1 : std::exp( (cmax-cmax_prime)/temperature ); }

int cool_temerature(int temperature)
{ 
    const float cooling_coeff = 0.98;
    return cooling_coeff * temperature;
}

std::vector<process> simulate_annealing(std::vector<process> & processes, int start_temperature, int end_temperature, int iterations)
{
    int current_temperature = start_temperature;
    for(int i=0; i < iterations; ++i)
    {
        auto index = generateRandomIndex(processes.size());
        auto current_processes = processes;
        std::iter_swap(current_processes.begin(),current_processes.begin()+index);
        
        auto probability = acceptanceProbabilty(maxspan(processes),maxspan(current_processes),current_temperature);
        
        if(probability > 0.8)
            { std::iter_swap(processes.begin(),processes.begin()+index); }

        current_temperature = cool_temerature(start_temperature);

        if(current_temperature < end_temperature)
            { return current_processes; }
    }

    return processes;
}

int main(void)
{
    auto input = utility::readFile("Data/ta088.csv");
    auto times = utility::createTimes(input);
    auto processes = utility::createProcesses(times);
    
    auto result = simulate_annealing(processes,maxspan(processes),6730,10000);
    std::cout << "Simulated Annealing\n";
    std::cout << maxspan(result) << "\n";
}