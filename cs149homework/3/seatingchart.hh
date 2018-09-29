#ifndef SEATINGCHART
#define SEATINGCHART

#include <array>
#include <pthread.h>
#include <semaphore.h>
#include <utility>
using namespace std;

#include "seller.hh"
#include "customer.hh"

struct Seller;
struct Customer;

struct SeatingChart {
    pthread_mutex_t mutex;
	int seatsFilled;
	array<int,10> mediumRowSearchOrder;
    //array<array<string, 10>, 10> chart;
    string chart[10][10];

	SeatingChart(); 
    bool reserveSeat(Seller& seller, Customer& customer); 
};

//forgive me
struct sellerStruct {
    Seller* mySeller;
    SeatingChart* myChart;
    EventQueue* myQueue;
};

struct customerStruct {
    Customer* myCustomer;
    SeatingChart* myChart;
    EventQueue* myQueue;
};

#endif
