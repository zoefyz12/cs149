#include "eventqueue.hh"

//TODO redefine this if need be
// you need to initialize the static
// member to be something in the cc file
// like this
int EventQueue::_timeSleep = 0;

//TODO fill in constructor if need be
EventQueue::EventQueue() {
    pthread_mutex_init(&_mutex, NULL);
}

// Adds an Event object into the queue. Will lock mutex 
// in order to be concurrent
void EventQueue::addEvent(Event anEvent) {
   pthread_mutex_lock(&_mutex);
   _events.push_back(anEvent);
   pthread_mutex_unlock(&_mutex);
}

// Will iterate over queue of events and remove them at each tick
// Amount of time in between updates should be 1 second
/*void EventQueue::printEvents() {
   _timeSleep = 0;
    pthread_mutex_lock(&_mutex);
   while (_timeSleep < 60) {
      while (!_events.empty()) {
         auto& event = _events.front();
         event.print();
         _events.pop_front();
      }
      sleep(SECOND);
      _timeSleep++;
   }
   pthread_mutex_unlock(&_mutex);
}*/

void EventQueue::nonThread() {
    pthread_mutex_lock(&_mutex);
    while (!_events.empty()) {
       auto& event = _events.front();
       event.print();
       _events.pop_front();
    }
    pthread_mutex_unlock(&_mutex);
}
