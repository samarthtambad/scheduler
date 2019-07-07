#include "Scheduler.h"

class RoundRobin : public Scheduler {
private:
    /* data */
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

}

Process* RoundRobin::get_next_process(){
    return nullptr;
}