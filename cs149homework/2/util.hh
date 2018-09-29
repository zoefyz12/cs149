#ifndef UTIL_HH
#define UTIL_HH

#include "schedule.hh"

// constants -----------------------------------------------------------------
const int UNINITIALIZED = -1;
const string ID_LIST = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
// ---------------------------------------------------------------------------

process* findEligibleProcess(deque<process>& processes, int currentQuanta) {
   if (currentQuanta >= 99) {
      return nullptr;
   }
   for (auto &p : processes) {
      // If process is still running and can be run during the current quanta return it.
      if (p.timeRan <= p.expectedRunTime && p.arrivalTime <= currentQuanta) {
         //cout << "[" << currentQuanta << "] " << p.name << " Ran: " << p.timeRan << " Expected: " << p.expectedRunTime << " Arrival: " << p.arrivalTime << endl;
         processes.erase(processes.begin());
         return &p;
      }
   }
   return nullptr;
}

vector<process> generateProcesses(float minRunTime) {
    vector<process> processes;
    
    // generate random numbers
    default_random_engine rand_engine(std::chrono::system_clock::now().time_since_epoch().count());
    uniform_real_distribution<float> arrival_distro(0.1,99.0);
    uniform_int_distribution<int> priority_distro(1,4);
    uniform_real_distribution<float> runtime_distro(0.1,10.0);

    auto totalruntime = 0.0f;
    auto iditer = 0;

    // push back enough processes to fill up minRunTime
    while(totalruntime < minRunTime) {
    	auto runtime = runtime_distro(rand_engine);
        auto currid = (iditer > ID_LIST.length() ? 
            ID_LIST.substr(iditer, iditer%ID_LIST.length()+1) : ID_LIST.substr(iditer,1));
        processes.push_back({   arrival_distro(rand_engine), 
                                runtime, 
                                priority_distro(rand_engine), 
                                0, 
                                UNINITIALIZED, 
                                UNINITIALIZED,
                                currid,
                                UNINITIALIZED });  
                                
    	totalruntime += runtime;
        iditer++;
    }

    // return processes sorted in ascending order based on arrival time
    sort(begin(processes), end(processes), 
        [](process p1, process p2) { return p1.arrivalTime < p2.arrivalTime;});
 
    return processes;
}

void calculateRunStatistics(vector<process> processes, runStatistics& stats) {
	float sumTurnaround = 0;
	float sumWaiting = 0;
	float sumResponse = 0;
	float sumCompleted = 0;
	for (auto& p: processes) {
		if (p.quantaFinished != -1) { //Makes sure it actually ran
			sumTurnaround += (p.quantaFinished - p.arrivalTime);
			sumWaiting += (p.quantaFinished - p.arrivalTime - p.expectedRunTime);
			sumResponse += (p.timeFirstRan - p.arrivalTime);
			sumCompleted++;
		}
	}
	stats.averageTurnaroundTime = sumTurnaround / sumCompleted;
	stats.averageWaitingTime = sumWaiting / sumCompleted;
	stats.averageResponseTime = sumResponse / sumCompleted;
	stats.throughput = sumCompleted;
}


#endif // UTIL_HH
