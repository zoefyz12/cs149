// Highest Priority First Scheduling Algorithms, Preemptime and Non

#ifndef HP_HH
#define HP_HH

#include "schedule.hh"

const string IDLE = "~";

runStatistics highestPriorityPreemptive(vector<process> processes);	
runStatistics highestPriorityNonPreemptive(vector<process> processes);
void ageProcesses(deque<process> queues[], int timeSlice);
void createPQs(vector<process>& all, deque<process> queues[]);

runStatistics highestPriorityPreemptive(vector<process> processes){
	runStatistics stats;
	deque<process> queues[4];
	createPQs(processes, queues);
	int timeSlice = 0;
	process* current = nullptr;

	vector<process> finalList;
	while (true) {
		ageProcesses(queues, timeSlice);
		//choose a process round robin  within each priority
		//Make sure to not select anything that hasn't been started past slide 99. In fact probably remove it then and there.
		bool foundAProcess = false;
		for (auto& q : queues)
		{
			if (foundAProcess) {
				break;
			}
			int numberOfChecksThroughQueue = 0;
			while (q.size() != 0) {
				if (q.front().timeFirstRan == -1 && timeSlice > 99) {
					q.pop_front();
				}
				else if (q.front().arrivalTime < timeSlice) {
					current = &q.front();
					foundAProcess = true;
					break;
				} else {
					//Cycle to the next one in the queue
					process toMove = q.at(0);
					queues[toMove.priority-1].pop_front();
					queues[toMove.priority-1].push_back(toMove);
					numberOfChecksThroughQueue++;
					if (numberOfChecksThroughQueue > q.size()) {
						//Looked through the whole queue and found nothing available
						break;
					}
				}
			}
		}
		if (!foundAProcess) {
			if (timeSlice < 99) {
				stats.timeChart.push_back(IDLE);
				timeSlice++;
				continue; //Idle for this 
			} else {
				//End the scheduling entirely because we just cleared every process
				break;
			}
		}

		//Update stats of the process
		if (current->timeFirstRan == -1) {
			current->timeFirstRan = timeSlice;
			current->timeRan = 0;
		}
		current->timeRan++;
		stats.timeChart.push_back(current->name);
		//Evaluate if done with this iteration
		if (current->timeRan >= current->expectedRunTime) {
			current->quantaFinished = timeSlice;
			process toPush = *current;
			finalList.push_back(toPush);
			queues[current->priority-1].pop_front(); //removes it from the queue so it won't be selected again to run later
		} else {
			//Move it to the back of the queue for round robin because its not done
			process toMove = queues[current->priority-1].at(0);
			queues[toMove.priority-1].pop_front();
			queues[toMove.priority-1].push_back(toMove);
		}
		//Evaluate if done with all iterations (needs to have no processes remaining and be after slice 99)
		if(timeSlice > 99) {
			bool processesRemaining = false;
			for (auto& q : queues) {
				if (q.size() > 0) {
					processesRemaining = true;
				}
			}
			if (!processesRemaining) {
				break; //End the scheduling because there are no processes left
			}
		}
		timeSlice++;
	}
	calculateRunStatistics(finalList, stats);
	return stats;
}

runStatistics highestPriorityNonPreemptive(vector<process> processes){
	vector<process> finalList;
	runStatistics stats;
	deque<process> queues[4];
	createPQs(processes, queues);
	int timeSlice = 0;
	process* current = nullptr;
	while (true) {
		ageProcesses(queues, timeSlice);
		if (current == nullptr) {
			//choose a process (this is easy because its non preemptive)
			for (auto& q : queues) {
				if (q.size() != 0) {
					if (q.front().arrivalTime < timeSlice) {
						current = &q.front();
						break;
					}
				}
			}
			if (current == nullptr) {
				stats.timeChart.push_back(IDLE);
				timeSlice++;
				continue; //Idle for this 
			}
		}
		//Update stats of the process
		if (current->timeFirstRan == -1) {
			current->timeFirstRan = timeSlice;
			current->timeRan = 0;
		}
		timeSlice++;
		current->timeRan++;
		stats.timeChart.push_back(current->name);
		//Evaluate if done with this process
		if (current->timeRan >= current->expectedRunTime) {
			current->quantaFinished = timeSlice;
			process toPush = *current;
			finalList.push_back(toPush);
			queues[current->priority-1].pop_front(); //removes it from the queue so it won't be selected again to run later
			current = nullptr;
		}
		//Evaluate if done with all iterations (also easy because its non preemptive)
		if(timeSlice > 99 && (current == nullptr || current->quantaFinished >= 99)) {
			break;
		}
	}
	calculateRunStatistics(finalList, stats);
	return stats;
}

//VERY IMPORTANT THAT THESE ARE PASSED BY REFERENCE NOT BY VALUE
void ageProcesses(deque<process> queues[], int timeSlice){
	for (int qIndex = 0; qIndex < 4; qIndex++) {
		auto& currentQ = queues[qIndex];
		if (currentQ.size() > 0) {
			if (currentQ.front().priority == 1) {
				 //Can't promote above the top priority, so skip this queue
				continue;
			}
			auto itr = begin(currentQ);

			while (itr != end(currentQ)) {
			    process currentP = *itr;
			    if (timeSlice - currentP.timeSliceItWasLastRan >= 5 && currentP.timeSliceItWasLastRan != -1) {
					//bump up the process priority field.
					currentP.priority--;
					//Remove it from this queue and move it to the correct one
					process copiedP = currentP;
					queues[qIndex-1].push_back(copiedP);
					itr = currentQ.erase(itr);
			    }
			    else {
			        ++itr; //progress iterator because 
				}
			}
		}
	}
}

//Not sure that the modifications will escape this scope
void createPQs(vector<process>& all, deque<process> queues[]) {
	for (process p : all)
	{
		queues[p.priority - 1].push_back(p);
	}
}

#endif //HP_HH
