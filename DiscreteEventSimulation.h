#include <vector>
#include <queue>

using namespace std;

struct Event{
    int id;
    int timestamp;
};

struct CompareEvent {
	bool operator()(Event* const &e1, Event* const &e2) {
		return e1->timestamp > e2->timestamp;
	}
};

class DiscreteEventSimulation {
    private:
        /* data */
        priority_queue<Event*, vector<Event*>, CompareEvent> event_queue;
    public:
        DiscreteEventSimulation(/* args */);
        ~DiscreteEventSimulation();
        void put_event(int num, int time);
        Event* get_event();
        void rm_event();
};

DiscreteEventSimulation::DiscreteEventSimulation(/* args */)
{
}

DiscreteEventSimulation::~DiscreteEventSimulation()
{
}

void DiscreteEventSimulation::put_event(int num, int time){
    Event* e = new Event();
    e->id = num;
    e->timestamp = time;
    event_queue.push(e);
}

Event* DiscreteEventSimulation::get_event(){
    if(event_queue.empty()) return nullptr;
    return event_queue.top();
}

void DiscreteEventSimulation::rm_event(){
    Event* e = event_queue.top();
    event_queue.pop();
    delete e;
}