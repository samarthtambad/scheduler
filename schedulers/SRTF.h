#include <queue>
#include "Scheduler.h"

struct CompareProcess {
	bool operator()(Process* const &p1, Process* const &p2) {
        // if(p1->rem == p2->rem){
        //     return false;
        // }
		return p1->rem > p2->rem;
	}
};

class SRTF : public Scheduler {
private:
    /* data */
    std::priority_queue<Process*, deque<Process*>, CompareProcess> RUN_QUEUE;
    int count;
public:
    SRTF(/* args */);
    ~SRTF();
    void add_process(Process*);
    Process* get_next_process();
};

SRTF::SRTF(/* args */){
    this->scheduler_type = "SRTF";
    this->is_preemptive = false;
    this->quantum = 0;
    this->maxprio = 4;
    this->count = 0;
}

SRTF::~SRTF(){

}

void SRTF::add_process(Process* proc){
    count++;
    // proc->count = count;
    RUN_QUEUE.push(proc);
}

Process* SRTF::get_next_process(){
    if(RUN_QUEUE.empty()) return nullptr;
    Process* proc = RUN_QUEUE.top();
    RUN_QUEUE.pop();    
    return proc;
}