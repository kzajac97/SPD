#ifndef UTILITY_HPP
#define UTILITY_HPP

#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <exception>
#include <random>
#include <functional>
#include <random>
#include <tuple>

#include "process.hpp"

namespace utility
{
    using numeric_t = int; //alias
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;

    // reads file with given name
    // returns vector of numeric(int) type
    std::vector<numeric_t> readFile(const std::string & filename);
    // creates 2D times vector from numeric type vector  
    std::vector<std::vector<numeric_t> > createTimes(const std::vector<numeric_t> & input);    
    // creates process objects from times 2D vector
    // where each row represents one process 
    std::vector<process> createProcesses(std::vector<std::vector<int> > times);
    //creates timespan for given process order
    std::vector<std::vector<int> > getTimespan(std::vector<process> processes);
    // generate random test data 
    // <parms> number of machines and processes to generate times for
    // default arguments with max and min distribution values 
    std::vector<std::vector<numeric_t> > generateRandomTimes(unsigned int nMachines, unsigned int nProcesses, int minValue, int maxValue);
    std::vector<std::vector<numeric_t> > generateRandomTimes(unsigned int nMachines, unsigned int nProcesses, int minValue=1, int maxValue=10);
    // <returns> process with smallest time on one of its machines
    std::tuple<process,int> getProcessWithSmallestTime(std::vector<process> processes);
    // create flowshop model as a directed weighted graph
    //graph_t getGraphTimespan(std::vector<std::vector<int> > timespan);
    // measures execution time of any schelduing algorithm
    // <params> algorithm is function taking a vector of process objects as arguments
    // <returns> chrono duration time in nanoseconds 
    // To print result -> getExecutionTime(functiom,processes).count()
    template <typename function_t>
    auto getExecutionTime(function_t algorithm,std::vector<process> processes)
    {
        high_resolution_clock::time_point start = high_resolution_clock::now();       
        algorithm(processes);
        high_resolution_clock::time_point end = high_resolution_clock::now();
        //casting chrono time point type to double and computing time function took to sort an array
        duration<double,std::nano> diff = std::chrono::duration_cast<duration<double,std::nano>> (end-start);

        return diff;
    }

    // <returns> arg when arg is > 0
    // 0 when arg is < 0 </returns>
    template <typename T>
    T relu(T arg) { return (arg > 0) ? arg : 0; }
    // get row sum of 2D vector 
    template <typename T>
    T getRowSum(std::vector<std::vector<T> > vector,unsigned int beginning, unsigned int end,unsigned int row)
    {
        T result;
        
        if(beginning > vector[row].size() || end > vector[row].size() || row > vector.size())
            { throw std::out_of_range("Index out of range!"); }
        
        for(unsigned int i=beginning; i <= end; ++i)
            { result += vector[row][i]; }

        return result;
    }
    // get column sum for 2D vector
    template <typename T>
    T getColSum(std::vector<std::vector<T> > vector,unsigned int beginning, unsigned int end,unsigned int col)
    {
        T result;

        if(beginning > vector.size() || end > vector.size() || col > vector[col].size())
            { throw std::out_of_range("Index out of range!"); }
        

        for(unsigned int i=beginning; i <= end; ++i)
            { result += vector[i][col]; }

        return result;
    }

    template<typename T>
    void printVector2D(std::vector<std::vector<T> > vector)
    {
        for(const auto row : vector)
        {
            for(const auto col : row)
                { std::cout << col << " "; }
            
            std::cout << "\n";
        }
    }
}

#endif