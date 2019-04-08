#include <iostream>
#include <string>

#include "process.hpp"
#include "utility.hpp"
#include "flowshop.hpp"
#include "neh.hpp"
#include "annealing.hpp"

int main(void)
{
    const std::vector<std::string> type = { "Data/ta060.csv","Data/ta072.csv","Data/ta088.csv",
                                            "Data/ta100.csv","Data/ta108.csv","Data/ta120.csv"};
   
    for(auto x : type)
    {
        auto input = utility::readFile(x);
        auto times = utility::createTimes(input);
        auto processes = utility::createProcesses(times);
        auto neh_result = neh(processes);
        auto sa_result = simulate_annealing(neh_result,1.0,0.0,0.8,50000,"Exponential","Swap");
        std::cout << "SA " << x << ": " << maxspan(sa_result) << "\n\n";
        input.clear();
        times.clear();
        processes.clear();
        neh_result.clear();
        sa_result.clear();
    }
}