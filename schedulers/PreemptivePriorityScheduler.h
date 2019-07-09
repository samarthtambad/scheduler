#include "Scheduler.h"

class PreemptivePriorityScheduler : public Scheduler {
private:
    /* data */
public:
    PreemptivePriorityScheduler(stime_t);
    PreemptivePriorityScheduler(stime_t, int);
    ~PreemptivePriorityScheduler();
    void add_process(Process*);
    Process* get_next_process();
};

PreemptivePriorityScheduler::PreemptivePriorityScheduler(stime_t quantum){
    this->scheduler_type = "PREPRIO " + std::to_string(quantum);
    this->is_preemptive = false;
    this->quantum = quantum;
    this->maxprio = 4;
}

PreemptivePriorityScheduler::PreemptivePriorityScheduler(stime_t quantum, int maxprio){
    this->scheduler_type = "PREPRIO " + std::to_string(quantum);
    this->is_preemptive = false;
    this->quantum = quantum;
    this->maxprio = maxprio;
}

PreemptivePriorityScheduler::~PreemptivePriorityScheduler(){
    
}

void PreemptivePriorityScheduler::add_process(Process* proc){

}

Process* PreemptivePriorityScheduler::get_next_process(){
    return nullptr;
}