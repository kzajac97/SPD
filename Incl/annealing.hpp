#ifndef ANNEALING_HPP
#define ANNEALING_HPP

#pragma once

#include <tuple>
#include <iostream>
#include <vector>

#include "process.hpp"
#include "utility.hpp"  

unsigned int generateRandomIndex(unsigned int size);
std::tuple<unsigned int,unsigned int> generateRandomIndices(unsigned int size);
float acceptanceProbabilty(int cmax, int cmax_prime, int temperature);
int linear_cooling(int start_temperature, int t);
std::vector<process> shortestTaskFirst(std::vector<process> & processes);
std::vector<process> longestTotalTimeFirstStable(std::vector<process> & processes);
std::vector<process> simulate_annealing(std::vector<process> & processes, int start_temperature, int end_temperature, int iterations);

#endif