// Round Robin (RR) Scheduling Algorithm

#ifndef RR_HH
#define RR_HH

#include "schedule.hh"
#include "hp.hh"
#include "util.hh"

// Will return statistics about the Round Robin algorithm
runStatistics roundRobin(vector<process> processes) {
   runStatistics stats;
   deque<process> queue;
   deque<process> running;
   vector<process> finalResults;
  
   for (auto &p : processes) {
      queue.push_back(p);
   }
   
   int quanta = 0;
   process *current = nullptr;
   do {
      current = findEligibleProcess(queue, quanta);
      // No new process found. Using a running queue
      if (current == nullptr && running.size() > 0) {
         current = &running.front();
         current->timeRan++;
         stats.timeChart.push_back(current->name);
         // If process finishes, add it to the final queue
         if (current->timeRan >= current->expectedRunTime) {
            current->quantaFinished = quanta;
            finalResults.push_back(*current);
            running.pop_front();
         }
         // Otherwise add it to the back
         else {
            running.push_back(*current);
            running.pop_front();
         }
      }
      // Process found from new queue
      else if (current != nullptr) {
         current->timeFirstRan = quanta;
         current->timeRan++;
         // If process finishes with one quanta, add it to results
         if (current->timeRan >= current->expectedRunTime) {
            current->quantaFinished = quanta;
            finalResults.push_back(*current);
         }
         // Otherwise add it to the running queue 
         else {
            running.push_back(*current);
         }
      }
      // No eligible process found. Idle.
      else {
         stats.timeChart.push_back(IDLE);
      }
      quanta++;
   } while (quanta < 99 || running.size() > 0) ;
   calculateRunStatistics(finalResults, stats); 
   return stats;
}
#endif // RR_HH
