#ifndef NEH_HPP
#define NEH_HPP

#pragma once

#include <iostream>
#include <vector>

#include "process.hpp"
#include "utility.hpp"
#include "flowshop.hpp"

std::vector<process> shortestTaskFirst(std::vector<process> & processes);
// returns permutation with longest total time of task on every machine
std::vector<process> longestTotalTimeFirst(std::vector<process> & processes);
// uses stable_sort (merge) instead of quick sort
std::vector<process> longestTotalTimeFirstStable(std::vector<process> & processes);
// finds index which gives smallest maxspan for current task insertion
unsigned int getShortestTaskTime(std::vector<process> & processes, process task);
// finds index which gives smallest maxspan for current task insertion
// reuses computation if there is a lot of processes
auto getShortestTaskTimeAccelerated(std::vector<process> & processes, process task);
auto getMostExpansiveTask(std::vector<process> & processes);
// implementation of neh schelduing algorithm
std::vector<process> neh(std::vector<process> & processes);
// neh with reused computations 
// Acceleration is about 20% faster than normal neh
std::vector<process> neh_accelerated(std::vector<process> & processes);
std::vector<process> neh_modified(std::vector<process> & processes);

#endif