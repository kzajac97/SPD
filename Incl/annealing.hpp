#ifndef ANNEALING_HPP
#define ANNEALING_HPP

#pragma once

#include <tuple>
#include <iostream>
#include <vector>
#include <exception>
#include <string>

#include "process.hpp"
#include "utility.hpp"  

// generates random index in range (0,size)
// uses uniform_int distribution
unsigned int generateRandomIndex(unsigned int size);
// generates two random indexes to be swapped 
// <returns> a tuple of two units 
std::tuple<unsigned int,unsigned int> generateRandomIndices(unsigned int size);
// returns acceptance probability for generated move
// P(X) is always 1 if move generates better maxspan
// P(X) depends of value of sigmoid function if maxspan
// of generated move is worse than previous
// <params>
// cmax - maxspan before change (swap or insert)
// cmax_prime - maxspan after change
// temperature - current temperature
// <returns> probability of acceptance of current move
double acceptanceProbabiltySigmoid(int cmax, int cmax_prime, double temperature);
//
double acceptanceProbabilty(int cmax, int cmax_prime, double temperature);
// runs simulated annealing algorithm
// <params>
// processes - vector of processes in random order (can be a result of neh or other algorithm)
// start_temperature - initial value of T, typically 1 
// end_temperature - final value of T, typically 0
// acceptance_probabilty - value of how likely currently generated move is to be accepted
// value in ragne (0,1), usually high P ~= 0.98
// iterations - number of iterations, usually large - 10000 (or more)
// colling type - string hashed to enum to choose cooling function
// linear, exp, hyperbolic or logarithm 
// move_type - string hashed to enum to choose cooling function
// swap generates two random indices and swaps their values in process vector
// insert generates two random indices and inserts the process 
// from first index to the second 
std::vector<process> simulate_annealing(std::vector<process> & processes, double start_temperature, double end_temperature, 
                                        double acceptance_probabilty, long int iterations, std::string cooling_type, std::string move_type);

// cooling functions

// F(T) = T0 - kT
// linear cooling is linear decreasing function
// cooling coeff should depend on number of iterations
// N - number of iterations
// <params>
// start_temperature (T0) - initiall value of function (usually 1 or N)
// T - current temperature, type int because it depends on current iteration
// cooling coeff (k) - sets how fast is the function decreasing usually 1/N
// <returns> current temperature as double, usually value if range (0,1) 
double linear_cooling(double start_temperature, int t, double cooling_coeff);
// exponential cooling function
// F(T) = T0 - n*T
// probability decreases exponentially to 0
// <params>
// T - current temperature, doesn't depend on iteration 
// type double (usually called) T = exp_cooling(T,k) 
// cooling_coeff - sets how fast the function decreases
// is very sensitive to small changes (of 0.0002 order)
// typically in range (0.8,1)
// <returns> current temperature as double, usually value if range (0,1) 
double exp_cooling(double t, double cooling_coeff);
// inverse cooling uses hyperbolic function 
// F(T) = T/(1 + Beta*T)
// <params>
// T - current temperature (depends on iteration)
// cooling_coeff (Beta) - sets how fast the function decreases
// Beta typically very small B ~ (0.001,0.01) 
// smaller beta will give slower cooling, quite sensitive
// <returns> current temperature as double, usually value if range (0,1) 
double inverse_cooling(double t, double cooling_coeff);
// usues logarithm function
// F(T) = c/ln(T + d)
// <params>
// T - current temperature (depends on iteration)
// cooling_const (c) - constant (usually 1)
// cooling_coeff (d) - shifting constat (usually 1 or 0)
// <returns> current temperature as double, usually value if range (0,1) 
double log_coolling(int t, double cooling_coeff, double cooling_const);

// util enums

enum cooling_t
{
    eLinear,
    eExp,
    eInv,
    eLog
};

enum move_t
{
    eSwap,
    eInsert
};

cooling_t hash_cooling(const std::string & inString);
move_t hash_move(const std::string & inString);

#endif