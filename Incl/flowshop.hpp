#ifndef FLOWSHOP_HPP
#define FLOWSHOP_HPP

#pragma once

#include <iostream>
#include <vector>
#include <tuple>

#include "process.hpp"
#include "utility.hpp"

std::tuple<std::vector<int>,std::vector<int>,std::vector<int> > get_rpq_times(std::vector<process> processes);
// computes permutations of given process vector
// <input> process vector 
// <returns> 2D vector containing process objects
// in every possible order (permutations)
std::vector<std::vector<process> > getPermutations(std::vector<process> processes);
std::vector<process> shortestTaskFirst(std::vector<process> & processes);
std::vector<process> johnson2machine(std::vector<process> processes);
std::vector<process> johnson3machine(std::vector<process> processes);
// computes time of tasks execution for given process order
// computes finish time of every task on every machine
// <returns> a vector of vectors holding every task finish time
std::vector<std::vector<int> > maxspan_vector(std::vector<process> processes);
// returns value of maxspan
int maxspan(std::vector<process> processes);
// accelerated vesrion of maxspan using times from previoulsy comupted task times
int maxspan_accelerated(std::vector<process> processes, unsigned int start_index, std::vector<std::vector<int> > current_tasks);
int rpq_maxspan(std::vector<process> processes);

#endif