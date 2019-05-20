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
    std::array<std::string, 9> files = {"Data/data000.txt","Data/data001.txt","Data/data002.txt",
                                        "Data/data003.txt","Data/data004.txt","Data/data005.txt",
                                        "Data/data006.txt","Data/data007.txt","Data/data008.txt"};
    std::array<std::vector<int>, 9> data;
    std::array<std::vector<std::vector<int> >, 9> times;
    std::array<std::vector<process>, 9> processes;
    std::array<std::vector<process>, 9> results;
    std::array<std::chrono::duration<double> ,9> timespan;

    std::cout << "CARLIER\n";
    for(unsigned int i=0; i < files.size(); ++i)
    {
        data[i] = utility::readFile(files[i]);
        times[i] = utility::createTimes(data[i]);
        processes[i] = utility::createProcesses(times[i]);
        results[i] = scharge(processes[i]);
        std::cout << "Data " << i << " " << rpq_maxspan(results[i]) << "\n";
        std::cout << "Schrage pmtn: " << schrage_pmtn(processes[i]) << "\n";
    }
}