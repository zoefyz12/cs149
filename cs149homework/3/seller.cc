#include "seller.hh"
#include "seatingchart.hh"

Seller::Seller(string _name, Price _price) {
    name = _name;
    price = _price;
    pthread_mutex_init(&myMutex, NULL);
}

//Arg should be a pair<SeatingChart, EventQueue>
void* Seller::threadLoop(void* arg) {
    /*auto cast = static_cast<pair<SeatingChart&, EventQueue&>*>(arg);
    pair<SeatingChart&, EventQueue&> myPair = *cast;
    SeatingChart& seatChart = myPair.first;
    EventQueue& queue = myPair.second;*/
    auto cast = static_cast<sellerStruct*>(arg);
    auto idgaf = cast;
    auto seatChart = idgaf->myChart;
    auto queue = idgaf->myQueue;
    auto seller = idgaf->mySeller;
    int second = 0;
    Customer* current;

    int timeWaited = 0;
    while (second < 60) {
        if (current == nullptr) {
            //Choose a customer from the queue if available
            if (customers.size() != 0) {
                current = customers.front();
                customers.pop();
                if (seatChart->reserveSeat(*this, *current))
                {
                    //Send event to the queue about the seat being assigned
                    Event assignedEvent(EventType::ASSIGNED, second, current->name, seatChart->chart);
                    queue->addEvent(assignedEvent);
                    current->timeSeatAssigned = second;
                    timeWaited = 1; //TODO Check for off by one error but I think its k
                } else {
                    //raise an event about customer leaving because of full house.
                    Event fullHouseEvent(EventType::FULL, second, current->name, seatChart->chart);
                    queue->addEvent(fullHouseEvent);
                    //Already left the queue so we just need to remove the reference
                    current = nullptr;
                }
            }
        } else {
            if (timeWaited >= current->timeElapsedDuringPurchase) {
                current->timePurchaseMade = second;
                //Send finished event
                Event finishedEvent(EventType::FINISHED, second, current->name, seatChart->chart);
                current = nullptr;
                queue->addEvent(finishedEvent);
            } else {
                timeWaited++;
            }
        }
        sleep(1);
        second++;
    }
	return nullptr;
}

void Seller::push(Customer cussed) {
    pthread_mutex_lock(&myMutex);
    Customer* ihatemylife = new Customer(cussed);
    if(ihatemylife) {
        customers.push(ihatemylife);
    }
    pthread_mutex_unlock(&myMutex);
}

ostream& operator<<(ostream& o, const Seller& s) {
    return o << s.name;	
}

