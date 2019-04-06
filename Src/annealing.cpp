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

double linear_cooling(double start_temperature, int t, double cooling_coeff) 
{ return (double) 1.0*start_temperature - (cooling_coeff * 1.0*t); }

double exp_cooling(double t, double cooling_coeff) 
{ return cooling_coeff * t; }

double inverse_cooling(double t, double cooling_coeff)
{ return t/(1 + (cooling_coeff * t) ); }

double log_coolling(int t, double cooling_coeff, double cooling_const)
{ return cooling_const/(std::log(1.0*t + cooling_coeff) ); }

cooling_t hash_cooling(const std::string & inString)
{
    if(inString == "Linear") { return eLinear; }
    if(inString == "Exponential") { return eExp; }
    if(inString == "Inverse") { return eInv; }
    if(inString == "Logarithm") { return eLog; }
    else { throw std::invalid_argument("Unrecognized option"); }
}

move_t hash_move(const std::string & inString)
{
    if(inString == "Swap") { return eSwap; }
    if(inString == "Insert") { return eInsert; }
    else { throw std::invalid_argument("Unrecognized option"); }
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

double acceptanceProbabilty(int cmax, int cmax_prime, double temperature)
{  
    return cmax > cmax_prime ? 1.0 : 1.0 - 2.0/(1.0 + std::exp(temperature/ ( 0.04 * (-(double)cmax+(double)cmax_prime) ) ));
}

std::vector<process> simulate_annealing(std::vector<process> & processes, double start_temperature, double end_temperature, 
                                        double acceptance_probabilty, long int iterations, std::string cooling_type, std::string move_type)
{
    double current_temperature = (double)start_temperature;
    std::vector<process> current_processes;
    for(long int i=0; i < iterations; ++i)
    {
        auto index = generateRandomIndices(processes.size());
        
        current_processes = processes;
        
        switch(hash_move(move_type))
        {
            case eSwap: 
                { std::swap(current_processes[std::get<0>(index)],current_processes[std::get<1>(index)]); }
            break;

            case eInsert: 
            {
                auto element = current_processes[std::get<0>(index)];
                current_processes.erase(current_processes.begin() + std::get<0>(index));
                current_processes.insert(current_processes.begin() + std::get<1>(index), element);
            }
            break;

            default: throw std::invalid_argument("Unrecognized option"); break;
        }

        double probability = acceptanceProbabilty(maxspan(processes),maxspan(current_processes),current_temperature);
        
        if(probability >= acceptance_probabilty)
            { std::swap(processes[std::get<0>(index)],processes[std::get<1>(index)]); }

        switch(hash_cooling(cooling_type))
        {
            case eLinear: current_temperature = linear_cooling(start_temperature,i,1/iterations); break;
            case eExp: current_temperature = exp_cooling(current_temperature,0.9998); break;
            case eInv: current_temperature = inverse_cooling(i,0.001); break;
            case eLog: current_temperature = log_coolling(i,1,0) ;break;
            default: throw std::invalid_argument("Unrecognized option"); break;
        }

        if(current_temperature < end_temperature)
            { return processes; }

        if(i % 1000 == 0)
            { std::cout << i/1000 << " " << maxspan(processes) << "\n"; }

        current_processes.clear();
    }

    return processes;
}