#include <iostream>

#include "process.hpp"
#include "utility.hpp"
#include "flowshop.hpp"
#include "neh.hpp"
#include "annealing.hpp"

int main(void)
{
    auto input = utility::readFile("Data/ta088.csv");
    auto times = utility::createTimes(input);
    auto processes = utility::createProcesses(times);
    auto neh_time = neh(processes);
    std::cout << "Neh: " << maxspan(neh_time) << "\n"; 
    auto result = simulate_annealing(neh_time,1.0,0.0,0.8,100000,"Exponential","Swap");
    std::cout << "Annealing: " << maxspan(result) << "\n";
}