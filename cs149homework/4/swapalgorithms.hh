#ifndef SWAP_ALGORITHMS_HH
#define SWAP_ALGORITHMS_HH

#include <algorithm>
#include <iterator>
#include <unordered_set>
#include <deque>
#include <vector>
#include <iostream>
#include <utility>
using namespace std;

#include "process.hh"
#include "swapstats.hh"

const unsigned MAX_SIZE = 64;
const unsigned RUNTIME = 60;
const char EMPTY_MEM_LOC = '.';

// Forward declarations
bool isEligible(process proc, int currentTime);
void print(vector<char> memMap);
void print(vector<string> mmap);
void pruneProcesses(vector<pair<process, int>>& processes, vector<char>& memMap);
int findBestHole(vector<char>& memMap, int size);
void writeMemory(int start, int end, char ch, vector<char>& memMap);

SwapStats firstFit(deque<process> processes) {
    unsigned timer = 0;
    SwapStats firstFitStats;
    firstFitStats.numberProcessesSwappedIn = 0;

    deque<process> ready;
    deque<pair<process,int>> running;
    vector<char> memMap(100,'.');   

    while(timer < RUNTIME) {
        // add any newly-available procs
        copy_if(begin(processes), end(processes), back_inserter(ready),
            [&](process p) { return p.arrivalTime <= timer;});
        
        // remove newly added procs or finished procs
        processes.erase(remove_if(begin(processes), end(processes), 
            [&] (process p) { return p.arrivalTime <= timer; } ), processes.end());

        // Find the first hole 
        auto first = find_if(begin(memMap), end(memMap), 
            [&](char c) { return c == EMPTY_MEM_LOC;} );
        
        auto second = find_if(first, end(memMap),
                [&](char c) { return c != EMPTY_MEM_LOC;} );
       
        // Find a suitable hole if first isn't big enough 
        bool memLocFound = false;
        bool remainingMem = true;
        while(!ready.empty() && !memLocFound && remainingMem) {
            if(ready[0].size <= distance(first,second) && distance(first,second) != 0){
                memLocFound = true;
                continue;
            } else if(distance(first,second) == 0){
                remainingMem = false;
                continue;
            }

            first = find_if(second, end(memMap), 
                [&](char c) { return c == EMPTY_MEM_LOC;} );
            second = find_if(first, end(memMap),
                [&](char c) { return c != EMPTY_MEM_LOC;} );
        }

        // If said hole was found
        if(memLocFound) {
            auto count = 0;
            transform(first, second, first, 
                [&](char c) { return count++ < ready[0].size? ready[0].id[0] : '.'; } ); 

            ready[0].startTime = timer;
            firstFitStats.numberProcessesSwappedIn++;
            running.push_back(make_pair(ready[0], count));
            ready.pop_front();    
        }

        // Remove finished processes and clear from memory
        unordered_set<char> toRemove;
        running.erase(remove_if(begin(running), end(running),
            [&](pair<process,int> p) { 
                bool rem = p.first.runTime <= (timer - p.first.startTime);
                if(rem) toRemove.insert(p.first.id[0]); return rem; }), end(running));
        transform(begin(memMap), end(memMap), begin(memMap),
            [&](char c) { if(toRemove.find(c) != end(toRemove)) return '.'; else return c; });

        print(memMap);
        timer++;
    }

    return firstFitStats;
}

SwapStats bestFit(deque<process> processes) {
    unsigned timer = 0;
    SwapStats bestFitStats;
	bestFitStats.numberProcessesSwappedIn = 0;
    deque<process> ready;
    deque<pair<process,int>> running;
    vector<char> memMap(100,'.');   

    while(timer < RUNTIME) {
        // add any newly-available procs
        copy_if(begin(processes), end(processes), back_inserter(ready),
            [&](process p) { return p.arrivalTime <= timer;});
        
        // remove newly added procs or finished procs
        processes.erase(remove_if(begin(processes), end(processes), 
            [&] (process p) { return p.arrivalTime <= timer; } ), processes.end());

        // Find the first hole 
        auto first = find_if(begin(memMap), end(memMap), 
            [&](char c) { return c == EMPTY_MEM_LOC;} );
        
        auto second = find_if(first, end(memMap),
            [&](char c) { return c != EMPTY_MEM_LOC;} );
       
        typedef pair<decltype(begin(memMap)), decltype(begin(memMap))> itrs; 
   
        // Find a suitable hole if first isn't big enough 
        bool remainingMem = true;
        bool memLocFound = false;
        vector<itrs> possibleHolesToPenetrate;
        while(!ready.empty() && remainingMem) {
            if(ready[0].size <= distance(first,second) && distance(first,second) != 0){
                possibleHolesToPenetrate.push_back(make_pair(first, second));
                memLocFound = true;
            } else if(distance(first,second) == 0){
                remainingMem = false;
                print(memMap);
                continue;
            }

            first = find_if(second, end(memMap), 
                [&](char c) { return c == EMPTY_MEM_LOC;} );
            second = find_if(first, end(memMap),
                [&](char c) { return c != EMPTY_MEM_LOC;} );
        }

        // If said hole was found
        if(memLocFound) {
            auto count = 0;
            
            auto memLoc = min_element(begin(possibleHolesToPenetrate), end(possibleHolesToPenetrate),
                  [&](itrs p1, itrs p2) { return distance(p1.first, p1.second) < distance(p2.first, p2.second);} );
            
            transform(memLoc->first, memLoc->second, memLoc->first, 
                [&](char c) { return count++ < ready[0].size? ready[0].id[0] : '.'; } ); 

            ready[0].startTime = timer;
            bestFitStats.numberProcessesSwappedIn++;
            running.push_back(make_pair(ready[0], count));
            ready.pop_front();    
        }

        // Remove finished processes and clear from memory
        unordered_set<char> toRemove;
        running.erase(remove_if(begin(running), end(running),
            [&](pair<process,int> p) { 
                bool rem = p.first.runTime <= (timer - p.first.startTime);
                if(rem) toRemove.insert(p.first.id[0]); return rem; }), end(running));
        transform(begin(memMap), end(memMap), begin(memMap),
            [&](char c) { if(toRemove.find(c) != end(toRemove)) return '.'; else return c; });

        print(memMap);
        timer++;
    }
    return bestFitStats;
}

SwapStats nextFit(deque<process> processes) {
    vector<pair<process, int>> running;
    SwapStats stats;
    int currentTime = 0;
    int numSwaps = 0;
    int iteratingPos = 0;
    vector<string> mem(100, ".");
    while (currentTime < 60) {
        //Go through the running queue to add the running time or remove
        for (int i = 0; i < running.size(); i++) {
            auto& runningPair = running[i];
            runningPair.second++;
            //If its ran enough, remove from memory and print
            if (runningPair.second >= runningPair.first.runTime) {
                //Reset memory to "." where it had been
                for (int inner = 0; inner < mem.size(); inner++) {
                    string s = mem[inner];
                    if (runningPair.first.id == s) {
                        mem[inner] = ".";
                    }
                }
                //Remove it from the running vector
                running.erase(running.begin() + i);
                //Keep same index because everything gets shifted
                i--;
            }
        }
        int currentHoleSize = 0;
        //Now check if any available processes can fit in memory
        process proc = processes[0];
        //If the first process in teh queue isn't ready to run, end the loop iteration
        if (proc.arrivalTime > currentTime) {
            currentTime++;
            print(mem);
            continue;
        }
        for (int loop = 0; loop < mem.size(); loop++) {
            int indexToMem = (loop + iteratingPos) % mem.size();
            if (loop + iteratingPos == 0) {
                currentHoleSize = 0;
            }
            if (mem[indexToMem] == ".") {
                currentHoleSize++;
            } else {
                currentHoleSize = 0;
            }
            //Check if the hole is now big enough
            if (currentHoleSize >= proc.size) {
                iteratingPos = indexToMem + 1;
                //Fill the hole
                int numFilled = 0;
                for (int begin = indexToMem - currentHoleSize + 1; begin < mem.size() && numFilled < proc.size; begin++) {
                    mem[begin] = proc.id;
                    numFilled++;
                }
                //Start it at 1 because it runs this iteration
                auto p = make_pair(proc, 0);
                running.push_back(p);
                //Erase it from regular set so its not ran again
                processes.pop_front();
                numSwaps++;
                break;
            }
        }
        print(mem);
        currentTime++;
    }
    stats.numberProcessesSwappedIn = numSwaps;
    return stats;
}

void print(vector<char> mmap) {
    for(auto c : mmap) {
        cout << c;
    }
    cout << endl;
}

void print(vector<string> mmap) {
    for(auto s : mmap) {
        cout << s;
    }
    cout << endl;
}

// Finds an eligible process if there is one
bool isEligible(process proc, int currentTime) {
   return proc.arrivalTime <= currentTime;
}

// Writes out to memory the given character, to either reset or set
void writeMemory(int start, int end, char ch, vector<char>& memMap) {
   while (start++ < end) {
      memMap[start] = ch;
   }
}

// Will find the properly sized hole based on best fit algorithm.
// Returns index of the given hole and -1 if no hole was found.
int findBestHole(vector<char>& memMap, int size) {
   int index = -1, finalIndex = -1, hole = 0, smallest = MAX_SIZE;

   for (int i = 0; i < memMap.size(); i++) {
      if (memMap[i] == '.' && hole < size) {
         // If this is first occurence set index
         if (index == -1) {
            index = i;
         }
         hole++;
      }
      // Check to see if reset is needed.
      else {
         // If the hole is smaller and can fit the size. Change it
         if (hole < smallest && hole >= size) {
            smallest = hole;
            finalIndex = index;
         }
         // Reset values to continue
         hole = 0;
         index = -1;
      }
   }
   return index;
}

void pruneProcesses(vector<pair<process, int>>& processes, vector<char>& memMap) {
   for (int i = 0; i < processes.size(); i++) {
      auto& pair = processes[i];
      auto& process = pair.first;
      
      // Process has finished and needs to be removed from memory and running list
      if (--process.runTime <= 0) {
         // Print map, since process completed
         print(memMap);
         writeMemory(pair.second, pair.second + process.size, '.', memMap); 
         processes.erase(processes.begin() + i);
         i--;
      }
   }
}

#endif //SWAP_ALGORITHMS_HH
