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

#include "annealing.hpp"
#include "flowshop.hpp"

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
    return cmax > cmax_prime ? 1 : (float)2.0/( (float)1.0 + std::exp((-(float)cmax+(float)cmax_prime)/(float)temperature) ); 
}

int linear_cooling(int start_temperature, int t)
{ 
    const float cooling_coeff = 1;
    return start_temperature - cooling_coeff * t;   
}

std::vector<process> simulate_annealing(std::vector<process> & processes, int start_temperature, int end_temperature, int iterations)
{
    int current_temperature = iterations;
    std::vector<process> current_processes;
    for(int i=0; i < iterations; ++i)
    {
        auto index = generateRandomIndices(processes.size());
        
        current_processes = processes;
        std::swap(current_processes[std::get<0>(index)],current_processes[std::get<1>(index)]);
        
        float probability = acceptanceProbabilty(maxspan(processes),maxspan(current_processes),current_temperature);
        
        if(probability >= 0.98)
            { std::swap(processes[std::get<0>(index)],processes[std::get<1>(index)]); }

        current_temperature = linear_cooling(start_temperature,i);
        current_processes.clear();
    }

    return processes;
}