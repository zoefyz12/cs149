#ifndef SCHEDULE_HH
#define SCHEDULE_HH

#include <vector>
#include <string>
using namespace std;

struct process {
	float arrivalTime;
	float expectedRunTime;
	int priority;
	int timeRan; //sum of amount of time it has been run in total
	int timeFirstRan;
	int quantaFinished;
	string name;
	int timeSliceItWasLastRan; //THIS IS NOT A SUM. THIS IS A TIMESTAMP
};

struct runStatistics {
	vector<string> timeChart;
	float averageTurnaroundTime; // Elapsed time from when a process enters queue to when it finishes
	float averageWaitingTime; //amount of time process waits in ready queue
	float averageResponseTime; //Initial wait time: time first ran - arrival time
	float throughput; //Number of processes that complete execution / number of quantums ran
};

#endif //SCHEDULE_HH
