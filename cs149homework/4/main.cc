// std:: include
#include <vector>
using std::vector;

#include <iostream>
// our includes
#include "util.hh"
#include "process.hh"
#include "swapstats.hh"
#include "swapalgorithms.hh"
#include "pagestats.hh"
#include "pagingalgorithms.hh"

const unsigned MM_SIZE = 100;
const vector<unsigned> PROCESS_SZS = {4, 8, 16, 32};

int main(int arg, char** argv) {
    vector<SwapStats> swapStats;
    vector<PageStats> pageStats;

    for(unsigned i = 0; i < 5; i++) { 
        auto processes = generateProcesses(1000.0f); 
		
		cout << "Running First Fit: " << endl;
		cout << "+-----------------------------------------------------+\n";
        swapStats.push_back(firstFit(processes));
		cout << "Running Next Fit: " << endl;
		cout << "+-----------------------------------------------------+\n";
		swapStats.push_back(nextFit(processes));
        cout << "Running Best Fit: " << endl;
		cout << "+-----------------------------------------------------+\n";
		swapStats.push_back(bestFit(processes));

        // run paging algorithms
        pageStats.push_back(lfu());
       	pageStats.push_back(lru());
        pageStats.push_back(mfu());
        pageStats.push_back(fifo());
        pageStats.push_back(rpick());
    }
    
    float individualSwapStats[3] = {0, 0, 0};
    float individualPageStats[5] = {0, 0, 0, 0, 0};

    for(int i = 0; i < 15; i++) {
        individualSwapStats[i%3] += (swapStats[i].numberProcessesSwappedIn / 5.0f);
    }

    for(int i = 0; i < 25; i++) {
        individualPageStats[i%5] += (pageStats[i].hitRatio / 5.0f);
    }

    float sumNumSwaps = 0.0f;
    float sumPageHits = 0.0f;

    for(auto s : swapStats) {
		sumNumSwaps += s.numberProcessesSwappedIn;   
    }

    for(auto p : pageStats) {
        sumPageHits += p.hitRatio;
    }

    string swapNames[3] = {"First Fit", "Next Fit", "Best Fit"};
    string pageNames[5] = {"LFU", "LRU", "MFU", "FIFO", "Random Pick"};

    for(int i = 0; i<3; i++){
        cout << swapNames[i] << " had an average number of swaps of: " << individualSwapStats[i] << endl;
    }

    for(int i =0; i<5; i++){
        cout << pageNames[i] << " had an average hit ratio of: " << individualPageStats[i] << endl;
    }

    cout << "Total Avg swaps: " << sumNumSwaps/swapStats.size() << endl;
    cout << "Total Avg page hit ratio: " << sumPageHits/pageStats.size() << endl;
}
