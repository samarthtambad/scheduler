#include <queue>
#include "Scheduler.h"

struct CompareProcess {
	bool operator()(pair<int, Process*> const &p1, pair<int, Process*> const &p2) {
        if(p1.second->rem == p2.second->rem){
            return p1.first > p2.first;
        }
		return p1.second->rem > p2.second->rem;
	}
};

class SRTF : public Scheduler {

private:
    std::priority_queue<pair<int, Process*>, deque< pair<int, Process*> >, CompareProcess> RUN_QUEUE;
    int count;

public:
    SRTF();
    ~SRTF();
    void add_process(Process*);
    Process* get_next_process();
};

SRTF::SRTF(){
    this->scheduler_type = "SRTF";
    this->scheduler_symbol = 'S';
    this->is_preemptive = false;
    this->quantum = 10000;
    this->maxprio = 4;
    this->count = 0;
}

SRTF::~SRTF(){

}

void SRTF::add_process(Process* proc){
    count++;
    RUN_QUEUE.push(make_pair(count, proc));
}

Process* SRTF::get_next_process(){
    if(RUN_QUEUE.empty()) return nullptr;
    Process* proc = RUN_QUEUE.top().second;
    RUN_QUEUE.pop();    
    return proc;
}