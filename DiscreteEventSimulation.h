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
    return event_queue.top();
}

void DiscreteEventSimulation::rm_event(){
    if(!event_queue.empty()){
        Event* e = event_queue.top();
        event_queue.pop();
        // delete e;
    }
}