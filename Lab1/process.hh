#ifndef PROCESS_HH
#define PROCESS_HH

#pragma once

#include <iostream>
#include <vector>

using process_time_t = int;

class process
{
    private:
    unsigned int _id; 
    std::vector<int> _times; //times on every machine

    public:
    process(unsigned int id, std::initializer_list<int> init) : _id(id), _times(init) {}
    process(unsigned int id, std::vector<int> init) : _id(id), _times(init) {}
    ~process(void) {};

    std::vector<int> get_time(void) { return this->_times; }
    int get_id(void) { return this->_id; }

    void set_time(int time,unsigned int index) { this->_times[index] = time; }
    void set_id(int id) { this->_id = id; }

    bool operator > (process proc) { return this->_id > proc.get_id() ? true : false; } 
    bool operator < (process proc) { return this->_id < proc.get_id() ? true : false; }
};

#endif