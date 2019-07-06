#include <vector>
#include <queue>
#include "classes/Event.h"
#include "misc/globals.h"

using namespace std;

struct CompareEvent {
	bool operator()(Event* const &e1, Event* const &e2) {
        if(e1->evtTimeStamp == e2->evtTimeStamp){
            return e1->count > e2->count;
        }
		return e1->evtTimeStamp > e2->evtTimeStamp;
	}
};

class DiscreteEventSimulation {
    private:
        /* data */
        priority_queue<Event*, deque<Event*>, CompareEvent> event_queue;
        int count;
    public:
        DiscreteEventSimulation(/* args */);
        ~DiscreteEventSimulation();
        void put_event(Event*);
        Event* get_event();
        void rm_event();
        stime_t get_next_event_time();
};

DiscreteEventSimulation::DiscreteEventSimulation(/* args */)
{
    count = 0;
}

DiscreteEventSimulation::~DiscreteEventSimulation()
{
}

void DiscreteEventSimulation::put_event(Event* e){
    count++;
    e->count = count;
    event_queue.push(e);
}

Event* DiscreteEventSimulation::get_event(){
    if(event_queue.empty()) return nullptr;
    Event* evt = event_queue.top();
    rm_event();
    return evt;
}

void DiscreteEventSimulation::rm_event(){
    if(!event_queue.empty()){
        event_queue.pop();
    }
}

stime_t DiscreteEventSimulation::get_next_event_time(){
    return event_queue.top()->evtTimeStamp;
}