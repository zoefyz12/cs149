#include "event.hh"

Event::Event(EventType type, unsigned timeCreated, string customer, 
             /*array<array<string, GRID_SIZE>, GRID_SIZE> seatingChart*/
             string seatingChart[10][10]) :
   _type(type), 
   _timeCreated(timeCreated),
   _customer(customer)
{
    for (int outer = 0; outer < 10; outer++) {
        for (int inner = 0; inner <10; inner++) {
            _seatingChart[inner][outer] = seatingChart[inner][outer];
        }
    }
}

// Printing function based on what type of event is being called.
// Prints out the seating chart if a sale was completed.
void Event::print() {
   cout << "[0:" << _timeCreated << "] ";
   switch (_type) {
      case EventType::ARRIVED:
         cout << "Customer " << _customer << " arrived in line" << endl;
         break;
      case EventType::ASSIGNED:
         cout << "Customer " << _customer << " was assigned a seat" << endl;
         break;
      case EventType::FINISHED:
         cout << "Customer " << _customer << " was sold a ticket" << endl;
         cout << "Current Board State" << endl;
         for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
               cout << "| " << _seatingChart[i][j] << " |";
            }
            cout << endl;
         }
         break;
      case EventType::FULL:
         cout << "No available seat found. Customer " << _customer << " has left the line." << endl;
         break;
   }
}

