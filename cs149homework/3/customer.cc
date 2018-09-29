#include "customer.hh"

Customer::Customer( unsigned _timeOfArrival, unsigned _timeElapsedDuringPurchase, 
                    Seller& _seller, string _name):
    seller(_seller),
    timeOfArrival(_timeOfArrival),
    timeElapsedDuringPurchase(_timeElapsedDuringPurchase),
    name(_name)
{
}

void Customer::getInLine(EventQueue& queue) {
    Customer copy(*this);
    seller.push(copy);
}

void* Customer::threadLoop(void* argt) {
    //auto cast = static_cast<pair<SeatingChart&, EventQueue&>*>(argt);
    //pair<SeatingChart&, EventQueue&> myPair = *cast;
    //SeatingChart& seatChart = myPair.first;
    //EventQueue& queue = myPair.second;
    auto cast = static_cast<customerStruct*>(argt);
    auto idgaf = *cast;
    auto seatChart = idgaf.myChart;
    auto queue = idgaf.myQueue;
	int currentSecond = 0;
	while (currentSecond < 60) {
		if (currentSecond == timeOfArrival) {
            getInLine(*queue);
			//Send event to queue
            Event enteredLineEvent(EventType::ARRIVED, currentSecond, name, seatChart->chart);
            queue->addEvent(enteredLineEvent);
			return nullptr;
		}
        sleep(1);
		currentSecond++;
	}
	return nullptr;
}

ostream& operator<<(ostream& o, const Customer& c) {
	return o << c.name;
}


