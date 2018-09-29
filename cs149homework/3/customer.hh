#ifndef CUSTOMER_HH
#define CUSTOMER_HH

#include "seller.hh"
#include "seatingchart.hh"

struct Seller;

struct Customer {
	string name;
	Seller& seller;
	unsigned timeOfArrival;
	unsigned timeElapsedDuringPurchase;
	unsigned timeSeatAssigned;
	unsigned timePurchaseMade;

	Customer(unsigned _timeOfArrival, 
			 unsigned _timeElapsedDuringPurchase, 
			 Seller& _seller, string _name);	
	void getInLine(EventQueue& queue);
	void* threadLoop(void* arg);
};


#endif
