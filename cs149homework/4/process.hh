#ifndef PROCESS_HH
#define PROCESS_HH

#include <vector>
#include <string>
using std::string;

struct process {
	string id;
    float arrivalTime;
    int startTime;
    int runTime;
	int size;
};

#endif //PROCESS_HH
