#include <iostream>
#include <deque>
#include <random>
#include <vector>
#include <algorithm>
#include <set>
#include <iomanip>

#include <chrono>
using namespace std;

// include each of the individual algorithms
#include "schedule.hh"
#include "algorithms.hh"

void printStats(vector<process> processes, vector<runStatistics> stats, string algoName);

// Main function
int main() {
    vector<runStatistics> fcfsStats, sjfStats, srtStats, rrStats, preHPFStats, nonPreHPFStats;
    runStatistics runStats;
    vector<vector<process>> testProcs = { generateProcesses(99.0f), generateProcesses(99.0f),
                                            generateProcesses(99.0f), generateProcesses(99.0f),
                                            generateProcesses(99.0f) };
                                             
    // Runs each algorithm 5 times then averages the statistics
    for (int i = 0; i < 5; i++) {
         runStats = firstComeFirstServe(testProcs[i]); // First Come First Serve
         fcfsStats.push_back(runStats); 
          
         runStats = shortestJobFirst(testProcs[i]); // Shortest Job First
         sjfStats.push_back(runStats); 
         
         runStats = shortestRemainingTime(testProcs[i]); // Shortest Runtime First
         srtStats.push_back(runStats); 
         
         runStats = roundRobin(testProcs[i]); // Round Robin
         rrStats.push_back(runStats);
         
         runStats = highestPriorityPreemptive(testProcs[i]); // Highest Priority Preemptive
         preHPFStats.push_back(runStats);
         
         runStats = highestPriorityNonPreemptive(testProcs[i]); // Highest Priority Non-Preemptive
         nonPreHPFStats.push_back(runStats); 
    }
   
    printStats(testProcs[0], fcfsStats, "First-Come-First-Serve");
    printStats(testProcs[0], sjfStats, "Shortest Job First    ");
    printStats(testProcs[0], srtStats, "Shortest Runtime First");
    printStats(testProcs[0], rrStats, "Round Robin           ");
    printStats(testProcs[0], preHPFStats, "Highest Priority (Pre)");
    printStats(testProcs[0], nonPreHPFStats, "Highest Priority (Non)");
}

void printStats(vector<process> processes, vector<runStatistics> stats, string algoName) {
    std::cout << std::fixed;
    std::cout << std::setprecision(4);
    
    cout << "==================================================================================== \n";
    cout << " \t\t\t" << algoName << "\t\t\t" << endl;
    cout << "==================================================================================== \n";
    cout << "Statistics for 5 runs: \n\n";

    float avgWait = 0.0f;
    float avgTurn = 0.0f;
    float avgResp = 0.0f; 
    float through = 0.0f;

    cout << "_________________________________________________________________" << endl;
    cout << "| Wait Time \t | Turnaround Time | Response Time | Throughput |" << endl;
    for (auto r : stats) {
         cout << "| " 
              << r.averageWaitingTime 
              << "\t | " << r.averageTurnaroundTime 
              << "\t  | " << r.averageResponseTime 
              <<  "\t  | " << r.throughput << "  \t|" << endl;
               
      avgWait += r.averageWaitingTime;
      avgTurn += r.averageTurnaroundTime;
      avgResp += r.averageResponseTime;
      through += r.throughput;
    }
   
    cout << "| " << avgWait / 5 << " \t | " 
        << avgTurn / 5 
        << "\t  | " 
        << avgResp / 5 
        << "\t  | " 
        << through / 5 
        << " \t| Averages" << endl;
   cout << " _________________________________________________________________\n\n" << endl;

   cout << "Single Run Explanation\n\n";
   cout << "Given the following processes:\n" << endl; 
   for(auto proc : processes) {
        cout << " (Name: " << proc.name  
             << ") (Arrival Time: " << proc.arrivalTime    
             << ") (Run Time: " << proc.expectedRunTime << ") \n"; 
   }

   cout << "\nWe get the following sequence of quantums, with ~ representing no process:\n\n ";
   int count = 0;
   for(auto line : stats[0].timeChart) {
        if(count > 80){
            cout << endl << " ";
            count = 0;
        }
        cout << line;
        count++;
   }
   cout << endl << endl;
   cout << "\n\n\n";
}
