#ifndef UTILITY_HH
#define UTILITY_HH

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

#include "process.hh"

namespace utility
{
    using numeric_t = int; //alias

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
    std::vector<std::vector<numeric_t> > generateRandomTimes(unsigned int nMachines, unsigned int nProcesses);
    // <returns> arg when arg is > 0
    // 0 when arg is < 0 </returns>
    template <typename T>
    T relu(T arg) { return (arg > 0) ? arg : 0; }
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