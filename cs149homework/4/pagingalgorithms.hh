#ifndef PAGING_ALGORITHMS_HH
#define PAGING_ALGORITHMS_HH

#include <unordered_map>
#include <deque>
#include <array>
#include <vector>
#include <iostream>
using namespace std;

#include "process.hh"
#include "pagestats.hh"
typedef std::chrono::system_clock clk;
const unsigned MIN_SIZE = -1;
const unsigned TOTAL_REFS = 100;

// Forward declarations
bool inQueue(int pageNumber, deque<pair<int, int>>& queue);
int evictLeastUsed(deque<pair<int, int>>& queue);
int evictMostUsed(deque<pair<int, int>>& queue);
void print(deque<pair<int, int>> deque);
void print(deque<int> pageLayout);

void printNewPageAlgoStart(string name) {
    cout << endl << name << " is starting to run now" << endl;
}
 
int pickNewPage(int current) {
    default_random_engine rand_engine(clk::now().time_since_epoch().count());
    //Determine if local
    uniform_int_distribution<int> rangeDistribution(0, 9);
    uniform_int_distribution<int> localDistribution(-1, 1);
    uniform_int_distribution<int> nonLocalDistribution(0, 6);
    int localityTest = rangeDistribution(rand_engine);
    if (localityTest < 3) {
        int temp = current + localDistribution(rand_engine);
        if (temp < 0) {
            return 0;
        } else if (temp > 9) {
            return 9;
        } else {
            return temp;
        }

    } else {
        int nonLocal = nonLocalDistribution(rand_engine); //0 to 6
        vector<int> nums = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        if (current-1 >= 0 && current-1 <= 9)
            nums.erase(find(nums.begin(), nums.end(),current-1));
        if (current >= 0 && current <= 9)
            nums.erase(find(nums.begin(), nums.end(),current));
        if (current+1 >= 0 && current+1 <= 9)
            nums.erase(find(nums.begin(), nums.end(),current+1));
        return nums[nonLocal];
    }
}

void printPages(deque<int> q) {
    cout << "pages are ";
    for (auto i : q) {
        cout << "[" << i << "]";
    }
    cout << endl;
}

PageStats fifo() {
    printNewPageAlgoStart("FIFO");
    default_random_engine rand_engine(clk::now().time_since_epoch().count());
    uniform_int_distribution<int> rangeDistribution(0, 9);
    deque<int> queue;
    float pagesHit = 0.0f;
    int currentPage = rangeDistribution(rand_engine);
    queue.push_back(currentPage);
    cout << currentPage << " was added to memory" << endl;
    for (int i = 0; i < 100; i++) {
        currentPage = pickNewPage(currentPage);
        //If its not in the queue
        if( find(queue.begin(), queue.end(), currentPage) == queue.end()) {
            int front;
            if (queue.size() == 4) {
                front = queue.front();
                cout << front << " was kicked out for " << currentPage << "\t";
                queue.pop_front();
            } else {
                cout << currentPage << " was added to memory" << endl;
            }
            queue.push_back(currentPage);
        } else {
            pagesHit++; //Don't need to do anything else
            cout << currentPage << " was already in memory" << "\t";
        }
        printPages(queue);
    }
    PageStats stats;
    stats.hitRatio = pagesHit / 100.0f;
    return stats;
}


PageStats rpick() {
    printNewPageAlgoStart("Random Pick");
    default_random_engine rand_engine(clk::now().time_since_epoch().count());
    uniform_int_distribution<unsigned> pages(0, 9);
	uniform_int_distribution<unsigned> qpick(0, 3);
    auto hits = 0.0f;
    int pgChoice = pages(rand_engine);
    deque<int> pgsInMem;

    for(auto i = 0; i < TOTAL_REFS; i++) {
        auto tmp = qpick(rand_engine);
        pgChoice = pickNewPage(pgChoice);
        if(find(begin(pgsInMem), end(pgsInMem), pgChoice) == end(pgsInMem)) {
            if(pgsInMem.size() == 4) {
				cout << pgChoice << " was swapped in for ";
        		cout << pgsInMem[tmp] << "\t";
				pgsInMem[tmp] = pgChoice;
			} else {
            	pgsInMem.push_back(pgChoice);
			}
		} else { 
			cout << pgChoice << " was already in memory ";
            hits++;
        }
        
		print(pgsInMem);
    }
    return  { hits/100.0f };
}

PageStats lru() {
    printNewPageAlgoStart("LRU");
    int hits = 0;
	int pgChoice = pickNewPage(4); // rand() == 4
    deque<int> pgsInMem;
	deque<int> lruTracker = {0,1,2,3,4,5,6,7,8,9};

    // TODO print about swapping in and stuff
    for(auto i = 0; i < TOTAL_REFS; i++) {
       	//TODO add print
		// page miss
		pgChoice = pickNewPage(pgChoice);
		if(find(begin(pgsInMem), end(pgsInMem), pgChoice) == end(pgsInMem)) {
			if(pgsInMem.size() == 4) {
				for(int i = lruTracker.size()-1; i >= 0; i--) {
					auto found = find(begin(pgsInMem), end(pgsInMem),lruTracker[i]); 
					if(found != end(pgsInMem)) {
						pgsInMem.erase(found);
						auto tmp = lruTracker[i];
						lruTracker.erase(begin(lruTracker) + i);
						lruTracker.push_front(tmp);
        				cout << tmp << " was swapped for " << pgChoice << "\t";
						break;
					}
				}	
			}  
            	
			pgsInMem.push_back(pgChoice);
			auto found = find(begin(lruTracker), end(lruTracker), pgChoice);
			lruTracker.erase(found);
			lruTracker.push_front(pgChoice);
			
        } else { // page hit
            cout << pgChoice << " was already in memory "; 
			hits++;
        	auto found = find(begin(lruTracker), end(lruTracker), pgChoice);
			lruTracker.erase(found);
			lruTracker.push_front(pgChoice);
		}

        cout << "pages are ";
        for(auto pg : pgsInMem) {
            cout << "[" << pg << "]";
        }
        cout << endl;
    }
    return  {hits/100.0f};
}

//TODO
PageStats lfu() {
    printNewPageAlgoStart("LFU");
    PageStats stats;
    default_random_engine rand_engine(clk::now().time_since_epoch().count());
    uniform_int_distribution<int> rangeDistribution(1, 10);
    int currentPage = rangeDistribution(rand_engine);
    float pagesHit = 0.00f;
    int time = 0, queueSize = 0;
    deque<pair<int, int>> queue;

    while (time++ < 100) {
        currentPage = pickNewPage(currentPage);
        // Page not in queue. Need to add.
        if (!inQueue(currentPage, queue)) {
            // Space still left. No need to evict page.
            if (queue.size() < 4) {
               queue.push_back(make_pair(currentPage, 1));
               queueSize++;
            }
            // No space. Need to evict a page.
            else {
               int kickedOut = evictLeastUsed(queue);
               cout << kickedOut << " was kicked out for " << currentPage << "\t";
               queue.push_back(make_pair(currentPage, 1));
               queueSize++;
            }
        }
        else {
            pagesHit++;
            cout << currentPage << " was already in memory" << "\t";
        }
        print(queue);
    }
    stats.hitRatio = pagesHit / 100.0f;
    return stats; 
}

//TODO
PageStats mfu() {
    printNewPageAlgoStart("MFU");
    PageStats stats;
    default_random_engine rand_engine(clk::now().time_since_epoch().count());
    uniform_int_distribution<int> rangeDistribution(1, 10);
    int currentPage = rangeDistribution(rand_engine);
    float pagesHit = 0.00f;
    int time = 0, queueSize = 0;
    deque<pair<int, int>> queue;

    while (time++ < 100) {
        currentPage = pickNewPage(currentPage);
        // Page not in queue. Need to add.
        if (!inQueue(currentPage, queue)) {
            // Space still left. No need to evict page.
            if (queue.size() < 4) {
               queue.push_back(make_pair(currentPage, 1));
               queueSize++;
            }
            // No space. Need to evict a page.
            else {
               int kickedOut = evictMostUsed(queue);
               cout << kickedOut << " was kicked out for " << currentPage << "\t";
               queue.push_back(make_pair(currentPage, 1));
               queueSize++;
            }
        }
        else {
            pagesHit++;
            cout << currentPage << " was already in memory" << "\t";
        }
        print(queue);
    }
    stats.hitRatio = pagesHit / 100.0f;
    return stats; 
}


void print(vector<int> pageLayout) {
    cout << "pages are ";
    for(auto i : pageLayout) {
        cout << "[" << i << "]";
    }
    cout << endl;
} 

void print(deque<int> pageLayout) {
    cout << "pages are ";
    for(auto i : pageLayout) {
        cout << "[" << i << "]";
    }
    cout << endl;
}

void print(deque<pair<int, int>> deque) {
   cout << "pages are ";
    for (auto i : deque) {
      cout << "[" << i.first << "]";
   }
   cout << endl;
}

// Checks queue for existing page. Will automatically update it if found.
bool inQueue(int pageNumber, deque<pair<int, int>>& queue) {
   for (auto& pagePair : queue) {
      if (pagePair.first == pageNumber) {
         pagePair.second++;
         return true;
      }
   }
   return false;
}

int evictLeastUsed(deque<pair<int, int>>& queue) {
   int lowestCount = MAX_SIZE;
   int indexToEvict = 0;
   for (int i = 0; i < queue.size(); i++) {
      auto pair = queue[i];
      // Found lesser used page. Need to note its location
      if (lowestCount > pair.second) {
         lowestCount = pair.second;
         indexToEvict = i;
      }
   }
   auto pr = *(queue.begin() + indexToEvict);
   int ret = pr.first;
   queue.erase(queue.begin() + indexToEvict);
   return ret;
}

int evictMostUsed(deque<pair<int, int>>& queue) {
   int highestCount = 0;
   int indexToEvict = 0;
   for (int i = 0; i < queue.size(); i++) {
      auto pair = queue[i];
      // Found more used page. Need to note its location
      if (highestCount < pair.second) {
         highestCount = pair.second;
         indexToEvict = i;
      }
   }
   auto pr = *(queue.begin() + indexToEvict);
   int ret = pr.first;
   queue.erase(queue.begin() + indexToEvict);
   return ret;
}
#endif //PAGING_ALGORITHMS_HH
