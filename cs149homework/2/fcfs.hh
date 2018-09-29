// First Come First Serve (FCFS) Scheduling Algorithm

#ifndef FCFS_HH
#define FCFS_HH

#include "schedule.hh"
#include "util.hh"

runStatistics firstComeFirstServe(vector<process> processes) {
   runStatistics stats;
   vector<process> results;
   vector<process> _processes = processes;
   deque<process> queue;
   for (auto pr : processes) {
      queue.push_back(pr);
   }
  
   int quanta = 0;
   while (quanta < 99 || queue.size() > 0) {
      process *p = &queue.front();
      
      // If a new processes was found add quanta to it until completion
      if (p->arrivalTime <= quanta) {
         
         // If this is the first time the process is run at the start time to it
         if (p->timeFirstRan == UNINITIALIZED) {
            p->timeFirstRan = quanta;
         }

         p->timeRan++;
         stats.timeChart.push_back(p->name);
         if (p->expectedRunTime <= p->timeRan) {
            // Increase counter to grab next process
            p->quantaFinished = quanta;
            process *resultProcess = &queue.front();
            results.push_back(*resultProcess);
            queue.pop_front();
         }
      } else {
         // No process found. Add idle time.
         stats.timeChart.push_back(IDLE);
      }
      quanta++;
   }
   calculateRunStatistics(results, stats);
   return stats;
}

#endif // FCFS_HH
