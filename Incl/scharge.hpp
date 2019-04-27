#ifndef SCHARGE_HPP
#define SCHARGE_HPP

#pragma once

#include <iostream>
#include <vector>

#include "process.hpp"
#include "flowshop.hpp"
#include "utility.hpp"

std::vector<process> sharge(std::vector<process> & processes);
int rpq_maxspan(std::vector<process> & processes);

#endif