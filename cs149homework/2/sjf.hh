// Shortest Job First (SJF) scheduling algorithm

#ifndef SJH_FF
#define SJH_FF

#include <queue>
#include <cmath>
#include <algorithm>

#include "schedule.hh"
#include "util.hh"

using namespace std;

const int quanta = 100;

runStatistics shortestJobFirst(vector<process> processes) {
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
            // compute statistics for process
            auto tmp = priority.top();
            tmp.timeRan = tmp.expectedRunTime;
            tmp.timeFirstRan = time;
            tmp.quantaFinished = time + ceil(tmp.expectedRunTime); 
            // add copy to vector, remove from queue
            completedProcesses.push_back(tmp);
            priority.pop();
            time++;
            // use ceil() to round to next quanta(can't start on fractional time)
            for(int i = time; i <= (time + ceil(tmp.expectedRunTime)); i++) {
                statistics.timeChart.push_back(tmp.name);
            }
            time += ceil(tmp.expectedRunTime);
        } else { 
            // if no process is run, just increment the time by one quanta.
            time++;
            statistics.timeChart.push_back("~"); 
        }
    }

    // aggregate statistics such as average wait time and put into stat object
    calculateRunStatistics(completedProcesses, statistics); 
    return statistics;
}

#endif // SJF_HH
