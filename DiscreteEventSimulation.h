#include <vector>
#include <queue>
#include <list>
#include "classes/Event.h"
#include "misc/globals.h"

using namespace std;

class DiscreteEventSimulation {
    private:
        /* data */
        list<Event*> events_list;
        int count;
    public:
        DiscreteEventSimulation(/* args */);
        ~DiscreteEventSimulation();
        void put_event(Event*);
        Event* get_event();
        void rm_event(int);
        Event* get_next_event_of_process(int);
        stime_t get_next_event_time();
        int get_num_of_events();
        void print_events_in_queue();
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
    list<Event*>::iterator it; 
    it = events_list.begin();
    while(it != events_list.end() && e->evtTimeStamp >= (*it)->evtTimeStamp) it++;
    events_list.insert(it, e);
}

Event* DiscreteEventSimulation::get_event(){
    if(events_list.empty()) return nullptr;
    Event* evt = events_list.front();
    events_list.pop_front();
    return evt;
}

Event* DiscreteEventSimulation::get_next_event_of_process(int pid){
    list<Event*>::iterator it; 
    it = events_list.begin();
    while(it != events_list.end() && (*it)->evtProcess->pid != pid) it++;
    if((*it)->evtProcess->pid != pid) return nullptr;
    return (*it);
}

void DiscreteEventSimulation::rm_event(int pid){
    list<Event*>::iterator it; 
    it = events_list.begin();
    while(it != events_list.end() && (*it)->evtProcess->pid != pid) it++;
    events_list.erase(it);
}

stime_t DiscreteEventSimulation::get_next_event_time(){
    if(events_list.empty()) return -1;
    return events_list.front()->evtTimeStamp;
}

int DiscreteEventSimulation::get_num_of_events(){
    return events_list.size();
}

void DiscreteEventSimulation::print_events_in_queue(){
    list<Event*>::iterator it; 
    printf("\n----------------------EVENTS----------------------\n");
    for(it = events_list.begin(); it != events_list.end(); ++it){
        printf("Event: %d, Process: %d, Timestamp: %d, State Transition: %d\n", (*it)->count, (*it)->evtProcess->pid, (*it)->evtTimeStamp, (*it)->transition);
    } 
    printf("---------------------------------------------------\n");
}