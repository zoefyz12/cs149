#ifndef UTIL_HH
#define UTIL_HH

// std:: includes
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <deque>

// our includes
#include "swapstats.hh"
#include "pagestats.hh"
#include "process.hh"

const int TOTAL_MEMORY = 100;
const int UNINITIALIZED = -1;
const string ID_LIST = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const int SIZES[4] = {5,11,17,31};

typedef std::chrono::system_clock clk;

deque<process> generateProcesses(float minRunTime) {
    using namespace std;
    deque<process> processes;
    
    // generate random numbers
    default_random_engine rand_engine(clk::now().time_since_epoch().count());
    uniform_real_distribution<float> arrival_distro(0.1,99.0);
    uniform_int_distribution<int> runtime_distro(1,12);
    uniform_int_distribution<int> memory_size_distro(0,3);

    auto totalruntime = 0.0f;
    auto iditer = 0;

    // push back enough processes to fill up minRunTime
    while(totalruntime < minRunTime) {
        // generates a unique, readable id for each process
        //auto currid = (iditer > ID_LIST.length() ? 
        //     ID_LIST.substr(iditer, iditer%ID_LIST.length()+1) 
        //    : ID_LIST.substr(iditer,1));
        
        
        processes.push_back({   ID_LIST.substr(iditer%ID_LIST.size(),1), //id
                                arrival_distro(rand_engine),//arrivalTime
                                UNINITIALIZED,              //startTime
                                runtime_distro(rand_engine),//runTime 
                                SIZES[memory_size_distro(rand_engine)] //size in MB   
        });  
                                
    	totalruntime += processes[processes.size()-1].runTime;
        iditer++;
    }

    // return processes sorted in ascending order based on arrival time
    sort(begin(processes), end(processes), 
        [](process p1, process p2) { return p1.arrivalTime < p2.arrivalTime;});
 
    return processes;
}

void printReport(const SwapStats& stats) {
    //TODO
}

void printReport(const PageStats& stats) {
    //TODO
}



#endif //UTIL_HH
