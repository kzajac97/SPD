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
//     std::array<std::string, 3> files = {"Data/in50.txt","Data/in100.txt","Data/in200.txt"};
//     std::array<std::vector<int>, 3> data;
//     std::array<std::vector<std::vector<int> >, 3> times;
//     std::array<std::vector<process>, 3> processes;
//     std::array<std::vector<process>, 3> results;

//     for(unsigned int i=0; i < files.size(); ++i)
//     {
//         data[i] = utility::readFile(files[i]);
//         times[i] = utility::createTimes(data[i]);
//         processes[i] = utility::createProcesses(times[i]);
//         results[i] = scharge(processes[i]);
//     }
    
//     for(auto x : results)
//         { std::cout << rpq_maxspan(x) << "\n"; }
    auto data = utility::readFile("Data/in00.txt");
    auto times = utility::createTimes(data);
    auto processes = utility::createProcesses(times);
    auto results = scharge(processes);
    std::cout << rpq_maxspan(results) << "\n";
}