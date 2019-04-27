#include <iostream>
#include <string>
#include <algorithm>
#include <array>

#include "process.hpp"
#include "utility.hpp"
#include "flowshop.hpp"
#include "neh.hpp"
#include "annealing.hpp"
#include "scharge.hpp"

int main(void)
{
    // std::array<std::string, 3> files = {"Data/in50.txt","Data/in100.txt","Data/in200.txt"};
    // std::array<std::vector<int>, 3> data;
    // std::array<std::vector<std::vector<int> >, 3> times;
    // std::array<std::vector<process>, 3> processes;
    
    // for(unsigned int i=0; i < files.size(); ++i)
    // {
    //     data[i] = utility::readFile(files[i]);
    //     times[i] = utility::createTimes(data[i]);
    //     processes[i] = utility::createProcesses(times[i]);
    // }

    // for(auto process : processes)
    //     { std::cout << rpq_maxspan(process) << "\n"; }
    auto data = utility::readFile("Data/ta000.csv");
    auto times = utility::createTimes(data);
    auto processes = utility::createProcesses(times);

    std::cout << rpq_maxspan(processes) << "\n";
}