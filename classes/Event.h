#include "Process.h"

#ifndef EVENT_H
#define EVENT_H
class Event {
// private:
public:
    /* data */
    int count;
    int id;
    Process* evtProcess;
    stime_t evtTimeStamp;
    process_state_t old_state;
    process_state_t new_state;
    state_transition_t transition;

    Event(Process* proc, stime_t tstamp, process_state_t old_s, process_state_t new_s, state_transition_t trans);
    ~Event();

};

Event::Event(Process* proc, stime_t tstamp, process_state_t old_s, process_state_t new_s, state_transition_t trans) {
    evtProcess = proc;
    evtTimeStamp = tstamp;
    old_state = old_s;
    new_state = new_s;
    transition = trans;
}

Event::~Event() {

}

#endif