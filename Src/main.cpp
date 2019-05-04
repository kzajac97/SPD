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

int main(void)
{
    std::array<std::string, 3> files = {"Data/in50.txt","Data/in100.txt","Data/in200.txt"};
    std::array<std::vector<int>, 3> data;
    std::array<std::vector<std::vector<int> >, 3> times;
    std::array<std::vector<process>, 3> processes;
    std::array<std::vector<process>, 3> results;
    std::array<std::chrono::duration<double> ,3> timespan;

    std::cout << "SHRAGE\n";
    for(unsigned int i=0; i < files.size(); ++i)
    {
        data[i] = utility::readFile(files[i]);
        times[i] = utility::createTimes(data[i]);
        processes[i] = utility::createProcesses(times[i]);
        auto start = std::chrono::high_resolution_clock::now();
        results[i] = scharge(processes[i]);
        auto end = std::chrono::high_resolution_clock::now();
        timespan[i] =  end - start;
    }

    for(auto x : results)
        { std::cout << rpq_maxspan(x) << "\n"; }  

    for(auto t : timespan)
        { std::cout << t.count() << "\n"; }
    
    // HEAP
    std::cout << "SHRAGE HEAP\n";
    for(unsigned int i=0; i < files.size(); ++i)
    {
        data[i] = utility::readFile(files[i]);
        times[i] = utility::createTimes(data[i]);
        processes[i] = utility::createProcesses(times[i]);
        auto start = std::chrono::high_resolution_clock::now();
        results[i] = scharge_heap(processes[i]);
        auto end = std::chrono::high_resolution_clock::now();
        timespan[i] =  end - start;
    }

    for(auto x : results)
        { std::cout << rpq_maxspan(x) << "\n"; }  

    for(auto t : timespan)
        { std::cout << t.count() << "\n"; }

    // HEAP
    std::cout << "SHRAGE HEAP FAST\n";
    for(unsigned int i=0; i < files.size(); ++i)
    {
        data[i] = utility::readFile(files[i]);
        times[i] = utility::createTimes(data[i]);
        processes[i] = utility::createProcesses(times[i]);
        auto start = std::chrono::high_resolution_clock::now();
        results[i] = scharge_heap_fast(processes[i]);
        auto end = std::chrono::high_resolution_clock::now();
        timespan[i] =  end - start;
    }

    for(auto x : results)
        { std::cout << rpq_maxspan(x) << "\n"; }  

    for(auto t : timespan)
        { std::cout << t.count() << "\n"; }

    // PMTN SCHRAGE TEST    
    std::array<int, 3> result;

    std::cout << "SHARGE PMTN\n";
    for(unsigned int i=0; i < files.size(); ++i)
    {
        data[i] = utility::readFile(files[i]);
        times[i] = utility::createTimes(data[i]);
        processes[i] = utility::createProcesses(times[i]);
        auto start = std::chrono::high_resolution_clock::now();
        result[i] = schrage_pmtn(processes[i]);
        auto end = std::chrono::high_resolution_clock::now();
        timespan[i] =  end - start;
    }
    
    for(auto x : result)
        { std::cout << x << "\n"; }

    for(auto t : timespan)
        { std::cout << t.count() << "\n"; }

    std::cout << "SHARGE PMTN HEAP\n";
    for(unsigned int i=0; i < files.size(); ++i)
    {
        data[i] = utility::readFile(files[i]);
        times[i] = utility::createTimes(data[i]);
        processes[i] = utility::createProcesses(times[i]);
        auto start = std::chrono::high_resolution_clock::now();
        result[i] = schrage_pmtn_heap(processes[i]);
        auto end = std::chrono::high_resolution_clock::now();
        timespan[i] =  end - start;
    }
    
    for(auto x : result)
        { std::cout << x << "\n"; }

    for(auto t : timespan)
        { std::cout << t.count() << "\n"; }

    // IMPOROVED RESULTS
    std::cout << "SHRAGE + ANNEALING\n";
    for(unsigned int i=0; i < files.size(); ++i)
    {
        data[i] = utility::readFile(files[i]);
        times[i] = utility::createTimes(data[i]);
        processes[i] = utility::createProcesses(times[i]);
        results[i] = scharge_heap(processes[i]);
        results[i] = simulate_rpq_annealing(results[i],1,0,0.9,40000,"Exponential","Swap");
    }

    for(auto x : results)
        { std::cout << rpq_maxspan(x) << "\n"; }  

}