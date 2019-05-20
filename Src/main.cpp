#include <iostream>
#include <string>
#include <algorithm>
#include <array>
#include <chrono>

#include "process.hpp"
#include "utility.hpp"
#include "flowshop.hpp"
#include "neh.hpp"
#include "annealing.hpp"
#include "scharge.hpp"
#include "carlier.hpp"

int main(void)
{
    // std::array<std::string, 3> files = {"Data/in50.txt","Data/in100.txt","Data/in200.txt"};
    // std::array<std::vector<int>, 3> data;
    // std::array<std::vector<std::vector<int> >, 3> times;
    // std::array<std::vector<process>, 3> processes;
    // std::array<std::vector<process>, 3> results;
    // std::array<std::chrono::duration<double> ,3> timespan;

    // std::cout << "CARLIER\n";
    // for(unsigned int i=0; i < files.size(); ++i)
    // {
    //     data[i] = utility::readFile(files[i]);
    //     times[i] = utility::createTimes(data[i]);
    //     processes[i] = utility::createProcesses(times[i]);
    //     results[i] = carlier(processes[i]);
    // }

    // std::cout << "MAXSPAN\n";
    // for(auto x : results)
    //     { std::cout << rpq_maxspan(x) << " "; }
    // std::cout << "\n";

    std::string files = "Data/in50.txt";
    auto data = utility::readFile(files);
    auto times = utility::createTimes(data);
    auto processes = utility::createProcesses(times);
    std::cout << rpq_maxspan(processes) << "\n";

    std::cout << std::get<1>(get_a_task(processes)) << " " << std::get<1>(get_b_task(processes)) << "\n";
    std::cout << std::get<1>(get_c_task(processes)) << "\n";    
}