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

#include "process.hh"

//namespace utility
//{
    using numeric_t = int; //alias

    // reads file with given name
    // returns vector of numeric(int) type
    std::vector<numeric_t> readFile(const std::string & filename);
    // creates 2D times vector from numeric type vector  
    std::vector<std::vector<numeric_t> > createTimes(const std::vector<numeric_t> & input);    
    // creates process objects from times 2D vector
    // where each row represents one process 
    std::vector<process> createProcesses(std::vector<std::vector<int> > times);
//}

#endif