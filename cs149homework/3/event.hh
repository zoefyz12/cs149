#ifndef EVENT_HH
#define EVENT_HH

#define GRID_SIZE 10

#include <array>
#include <string>
#include <iostream>

using namespace std;

/*
 * An Object that will be able to store certain information
 * about an event. The events types are specified in the assignment.
 * It will also handle printing out to Standard Output when needed.
 */

// Enums for the different types of events that can occur.
// Descriptions for each are int the homework documentation
enum class EventType{ARRIVED, ASSIGNED, FULL, FINISHED};

class Event {
private:
   EventType _type;
   unsigned _timeCreated;
   string _customer;
   //array<array<string, GRID_SIZE>, GRID_SIZE> _seatingChart;
    string _seatingChart[10][10];

public:
   Event(EventType type, unsigned timeCreated, string customer, 
         //array<array<string, GRID_SIZE>, GRID_SIZE> seatingChart);
         string seatingChart[10][10]);
   void print();
};
#endif //EVENT_HH
