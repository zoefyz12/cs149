#ifndef SELLER_HH
#define SELLER_HH

#include <queue>
#include <string>
#include <mutex>
#include <iostream>
#include <unistd.h>

#include "eventqueue.hh"
#include "customer.hh"

using namespace std;

struct Customer;
enum class Price { LOW, MEDIUM, HIGH };

struct Seller {
	string name;
	Price price;
	queue<Customer*> customers;
    pthread_mutex_t myMutex;

	Seller(string _name, Price _price);
    void push(Customer cussed);
	void* threadLoop(void* arg); 
};

#endif //SELLER_HH
