#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <queue>
//#include <iostream>

using namespace std;

struct Event{
    int id;
    int timestamp;
};

struct CompareEvent {
	bool operator()(Event* e1, Event* e2) {
		// return "true" if "p1" is ordered before "p2", for example:
		return e1->timestamp < e2->timestamp;
	}
};

priority_queue<Event*, vector<Event*>, CompareEvent> event_queue;


void put_event(int num, int time){
    Event e;
    e.id = num;
    e.timestamp = time;
    event_queue.push(&e);
}

Event* get_event(){
    if(event_queue.empty()) return nullptr;
    Event* e =  event_queue.top();
    return e;
}

void rm_event(){
    Event* e = event_queue.top();
    event_queue.pop();
    delete e;
}

int main(int argc, char *argv[]){

    put_event(1, 6);
    put_event(2, 10);
    put_event(3, 15);
    put_event(4, 18);
    put_event(6, 53);
    put_event(5, 40);



    printf("Event %d", get_event()->id);
    printf("Event %d", get_event()->id);

//    Event* evt;
//    while((evt = get_event())){
//    	printf("Event %d : %d", evt->id, evt->timestamp);
//    	rm_event();
//    }


    return 0;
}
