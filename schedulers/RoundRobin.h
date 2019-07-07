#include <queue>
#include "Scheduler.h"

class RoundRobin : public Scheduler {
private:
    std::deque<Process*> RUN_QUEUE;
public:
    RoundRobin(stime_t);
    ~RoundRobin();
    void add_process(Process*);
    Process* get_next_process();
};

RoundRobin::RoundRobin(stime_t quantum){
    this->scheduler_type = "RR" + std::to_string(quantum);
    this->is_preemptive = false;
    this->quantum = quantum;
    this->maxprio = 4;
}

RoundRobin::~RoundRobin(){

}

void RoundRobin::add_process(Process* proc){
    if(proc->state_ts){   // preempted. add at front.
        RUN_QUEUE.push_front(proc);
    } else{ // blocked. add at back.
        RUN_QUEUE.push_back(proc);
    }
}

Process* RoundRobin::get_next_process(){
    if(RUN_QUEUE.empty()) return nullptr;
    Process* proc = RUN_QUEUE.front();
    RUN_QUEUE.pop_front();    
    return proc;
}