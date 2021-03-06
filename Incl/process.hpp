#ifndef PROCESS_HPP
#define PROCESS_HPP

#pragma once

#include <iostream>
#include <vector>
#include <numeric>

using process_time_t = int;

class process
{
    private:
    unsigned int _id; 
    std::vector<int> _times; //times on every machine

    public:
    process() = default;
    process(process && proc) = default;
    process(const process & proc) = default;
    process(unsigned int id, std::initializer_list<int> init) : _id(id), _times(init) {}
    process(unsigned int id, std::vector<int> init) : _id(id), _times(init) {}
    ~process(void) {};

    const std::vector<int> get_time(void) { return this->_times; }
    const unsigned int get_id(void) { return this->_id; }

    void set_time(int time,unsigned int index) { this->_times[index] = time; }
    void push_times(int value) { this->_times.push_back(value); } 
    void pop_times(void) { this->_times.pop_back(); }
    void set_id(int id) { this->_id = id; }
    void print_times(void) { for(const auto x : this->_times) { std::cout << x << " "; } std::cout << "\n"; }

    int getExecutionTime(void) { return std::accumulate(this->_times.begin(),this->_times.end(),0); }

    bool operator > (process proc) { return this->_id > proc.get_id() ? true : false; } 
    bool operator < (process proc) { return this->_id < proc.get_id() ? true : false; }
    process & operator = (process && proc) = default;
    process & operator = (const process & proc) = default;
};

#endif