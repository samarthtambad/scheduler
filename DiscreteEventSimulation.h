#include <vector>
#include <queue>
#include "misc/globals.h"

using namespace std;

struct Event{
    int id;
    stime_t timestamp;
    int count;
};

struct CompareEvent {
	bool operator()(Event* const &e1, Event* const &e2) {
        if(e1->timestamp == e2->timestamp){
            return e1->count > e2->count;
        }
		return e1->timestamp > e2->timestamp;
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
        void put_event(int num, stime_t time);
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

void DiscreteEventSimulation::put_event(int num, int time){
    count++;
    Event* e = new Event();
    e->id = num;
    e->timestamp = time;
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
        delete e;
    }
}