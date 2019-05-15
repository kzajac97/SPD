#ifndef CARLIER_HPP
#define CARLIER_HPP

#pragma once

#include <iostream>
#include <vector>
#include <tuple>

#include "process.hpp"
#include "flowshop.hpp"
#include "utility.hpp"
#include "scharge.hpp"

std::vector<process> carlier(std::vector<process> & processes);
// Finds task with lowest index in given permutation so there
// are no breaks in scheldue between A and B tasks
// <returns> tuple with found task and it's index in processes vector
std::tuple<process,unsigned> get_a_task(std::vector<process> processes);
// Finds task with highest index in given permutation with Q (delivery) time
// larger than current maxspan 
// <returns> tuple with found task and it's index in processes vector
std::tuple<process,unsigned> get_b_task(std::vector<process> processes);
// Finds task in between tasks A and B with highest index and Q (delivery) time
// larger than B task delivery time, C task can be empty if A and B are next to each other
// or no task in between has delivery time grater than B task
// <returns> tuple with found task and it's index in processes vector and bool if task was found
std::tuple<process,unsigned,bool> get_c_task(std::vector<process> processes);

#endif