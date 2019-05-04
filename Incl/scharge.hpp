#ifndef SCHARGE_HPP
#define SCHARGE_HPP

#pragma once

#include <iostream>
#include <vector>
#include <tuple>

#include "process.hpp"
#include "flowshop.hpp"
#include "utility.hpp"

std::vector<process> scharge(std::vector<process> processes);
int schrage_pmtn(std::vector<process> processes);
auto get_rpq_times(std::vector<process> processes);
unsigned int getMaxIndex(std::vector<process> processes);
unsigned int getMinIndex(std::vector<process> processes);
int getMin(std::vector<process> processes);
process getRmin(std::vector<process> processes);
process getQmax(std::vector<process> processes);
int rpq_maxspan(std::vector<process> processes);

#endif