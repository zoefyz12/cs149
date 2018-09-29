#ifndef EVENT_QUEUE_HH
#define EVENT_QUEUE_HH
#define SECOND 1

#include <deque>
#include <pthread.h>
#include <unistd.h>
#include "event.hh"

using namespace std;

/*
 * The class that will be able to hold events that occur
 * during the runtime of the program and print them out 
 * in order as needed. This way printing is offloaded to
 * another thread and should hopefully keep everything concurrent
 * with a mutex locking scheme.
 */

class EventQueue {
private:
	pthread_mutex_t _mutex;
   deque<Event> _events;
   static int _timeSleep;
   
public:
   EventQueue();
   void addEvent(Event anEvent);
   //void printEvents();
   void nonThread();
};
#endif //EVENT_QUEUE_HH
