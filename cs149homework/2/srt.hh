// Shortest Remaining Time (SRT) scheduling algorithm

#ifndef SRT_HH
#define SRT_HH

#include <queue>
#include <cmath>
#include <algorithm>

#include "schedule.hh"
#include "util.hh"

using namespace std;

runStatistics shortestRemainingTime(vector<process> processes) {
    int time = 0;   
    runStatistics statistics;
    vector<process> completedProcesses;
    priority_queue<process, vector<process>, function<bool(process,process)>> priority (
        [](process p1, process p2) { return p1.expectedRunTime > p2.expectedRunTime;});

    // so long as processes remain or the time is under 99 
    while(processes.size() > 0 || time < quanta) {
        for(auto elt : processes) 
            if(elt.arrivalTime <= time)
                priority.push(elt);
        
        // remove processes which are eligible to begin
        processes.erase(remove_if(begin(processes), end(processes), 
            [&](process p) {return p.arrivalTime <= time;}), end(processes));
 
        if(priority.size() > 0) {
            auto tmp = priority.top();
            priority.pop();
            
            // set its start quanta if nec.
            if(tmp.timeFirstRan == -1) {
                tmp.timeFirstRan = time;
                tmp.timeRan = 0.0f;
            }

            // always increment time and timeRan
            tmp.timeRan++;
            statistics.timeChart.push_back(tmp.name);
            time++;
              
            // if this is its last timestep needed to complete then its done
            if( ((float)(tmp.expectedRunTime - tmp.timeRan)) <= 1.00f) {
                time++;
                tmp.quantaFinished = time;
                completedProcesses.push_back(tmp);     
            } else {
                priority.push(tmp);
            }
        } else {
            // if there is nothing in the priority queue to take from, just increment time
            time++; 
            statistics.timeChart.push_back("~");
        }
    }

    // aggregate statistics such as average wait time and put into stat object
    calculateRunStatistics(completedProcesses, statistics); 
    return statistics;
}

#endif //SRT_HH
