#include "Scheduler.h"

class PriorityScheduler : public Scheduler {
private:
    /* data */
public:
    PriorityScheduler(stime_t);
    PriorityScheduler(stime_t, int);
    ~PriorityScheduler();
    void add_process(Process*);
    Process* get_next_process();
};

PriorityScheduler::PriorityScheduler(stime_t quantum){
    this->scheduler_type = "PRIO" + std::to_string(quantum);
    this->is_preemptive = false;
    this->quantum = quantum;
    this->maxprio = 4;
}

PriorityScheduler::PriorityScheduler(stime_t quantum, int maxprio){
    this->scheduler_type = "PRIO" + std::to_string(quantum);
    this->is_preemptive = false;
    this->quantum = quantum;
    this->maxprio = maxprio;
}

PriorityScheduler::~PriorityScheduler(){
    
}

void PriorityScheduler::add_process(Process* proc){

}

Process* PriorityScheduler::get_next_process(){
    return nullptr;
}