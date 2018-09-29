#include <thread>
#include <cassert>
#include <ctime>
#include <iostream>
#include <vector>
#include <utility>
#include <sstream>
#include <random>
#include <unordered_map>
using namespace std;

#include "customer.hh"
#include "seller.hh"
#include "seatingchart.hh"

typedef uniform_int_distribution<unsigned> randist;
typedef chrono::system_clock clk;
void* runSeller(void* arg);
void* runBuyer(void* arg);
void* runQueue(void* arg);

pair<vector<Seller>, vector<Customer>> generateCustomers(unsigned n) {
   default_random_engine rand_engine(clk::now().time_since_epoch().count());
   randist arrival_distro(1,59);
   randist low_distro(4,7);
   randist medium_distro(2,4);
   randist high_distro(1,2);

   vector<Customer> customers;
   vector<Seller> sellers = { 	
				{"H1",Price::HIGH},	    {"M1",Price::MEDIUM}, 
				{"M2",Price::MEDIUM},	{"M3",Price::MEDIUM},
				{"L1",Price::LOW}, 		{"L2",Price::LOW},
				{"L3",Price::LOW}, 		{"L4",Price::LOW},
				{"L5",Price::LOW}, 		{"L6",Price::LOW}   
   };

   auto choose = [&](Price p) {
      if(p == Price::HIGH) {
         return high_distro(rand_engine);
      } else if(p == Price::MEDIUM) {
         return medium_distro(rand_engine);
      } else { 
      	return low_distro(rand_engine);
      }
   };

   for(unsigned j = 0; j < sellers.size(); j++) {
	    for(unsigned k = 0; k < n; k++) {		
		    customers.push_back( {
					arrival_distro(rand_engine),
					choose(sellers[j].price) , 
					sellers[j],
					(sellers[j].name + ((k >= 10) ? to_string(k) : "0" + to_string(k)))
		    });
	    }				
   }	
	
   return make_pair(sellers, customers);
} 

int main(int argc, char** argv) {
	assert(argc == 2);
	unsigned n;
	istringstream(argv[1]) >> n;
	auto people = generateCustomers(n);
	vector<Seller> sellers = people.first;
    vector<Customer> customers = people.second;
    EventQueue* queue = new EventQueue();
    SeatingChart* chart = new SeatingChart();

	//Create queue thread
    /*(pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, runQueue, queue);*/

	//create seller threads
    for (auto& sell : sellers) {
        //tuple<SeatingChart*, EventQueue*, Seller*> sellerArgs = make_tuple(&chart, &queue, &sell);
       sellerStruct* sellerArgs = new sellerStruct();
        sellerArgs->myChart = chart;
        sellerArgs->myQueue = queue;
        sellerArgs->mySeller = &sell;
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tid, &attr, runSeller, sellerArgs);
	}

    //create buyer threads
    for (auto& customer : customers) {
        /*tuple<SeatingChart*, EventQueue*, Customer*> custArgs = make_tuple(&chart, &queue, &buyer);*/
        customerStruct* custArgs = new customerStruct();
        custArgs->myChart = chart;
        custArgs->myCustomer = &customer;
        custArgs->myQueue = queue;
		pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tid, &attr, runBuyer, custArgs);
	}
    int _timeSleep = 0;
    while (_timeSleep < 60) {
        queue->nonThread();
        sleep(1);
        _timeSleep++;
    }
}

void* runSeller(void* arg) {
    static_cast<sellerStruct*>(arg)->mySeller->threadLoop(arg);
	return nullptr;
}

void* runBuyer(void* arg) {
    //auto ptr = static_cast<tuple<SeatingChart*, EventQueue*, Customer*>*>(arg);
    //auto ptr = static_cast<customerStruct*>(arg);
    //auto tupl = *ptr;
    /*SeatingChart& chart = *get<0>(tupl);
	EventQueue& queue = *get<1>(tupl);
    Customer& buyer = *get<2>(tupl);*/
    /*auto chart = tupl.myChart;
    auto queue = tupl.myQueue;
    auto customer = tupl.myCustomer;*/
    //auto pr = make_pair(chart, queue);
    static_cast<customerStruct*>(arg)->myCustomer->threadLoop(arg);
	return nullptr;
}

/*
void* runQueue(void* arg) {
	auto ptr = static_cast<EventQueue*>(arg);
	auto queue = *ptr;
	queue.printEvents();
	return nullptr;
}*/
