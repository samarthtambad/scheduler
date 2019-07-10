#include "Scheduler.h"

class PreemptivePriorityScheduler : public Scheduler {
private:
    /* data */
    std::queue<Process*> *ACTIVE_RUN_QUEUE;
    std::queue<Process*> *EXPIRED_RUN_QUEUE;
public:
    PreemptivePriorityScheduler(stime_t);
    PreemptivePriorityScheduler(stime_t, int);
    ~PreemptivePriorityScheduler();
    void add_process(Process*);
    Process* get_next_process();
};

PreemptivePriorityScheduler::PreemptivePriorityScheduler(stime_t quantum){
    this->scheduler_type = "PREPRIO " + std::to_string(quantum);
    this->scheduler_symbol = 'E';
    this->is_preemptive = false;
    this->quantum = quantum;
    this->maxprio = 4;
    ACTIVE_RUN_QUEUE = new std::queue<Process*>[this->maxprio];
    EXPIRED_RUN_QUEUE = new std::queue<Process*>[this->maxprio];
}

PreemptivePriorityScheduler::PreemptivePriorityScheduler(stime_t quantum, int maxprio){
    this->scheduler_type = "PREPRIO " + std::to_string(quantum);
    this->scheduler_symbol = 'E';
    this->is_preemptive = false;
    this->quantum = quantum;
    this->maxprio = maxprio;
    ACTIVE_RUN_QUEUE = new std::queue<Process*>[this->maxprio];
    EXPIRED_RUN_QUEUE = new std::queue<Process*>[this->maxprio];
}

PreemptivePriorityScheduler::~PreemptivePriorityScheduler(){
    
}

void PreemptivePriorityScheduler::add_process(Process* proc){
    if(proc->dynamic_prio == -1){
        proc->dynamic_prio = proc->static_prio - 1;
        EXPIRED_RUN_QUEUE[proc->dynamic_prio].push(proc);
    } else {
        ACTIVE_RUN_QUEUE[proc->dynamic_prio].push(proc);
    }
}

Process* PreemptivePriorityScheduler::get_next_process(){
    int prio;
    prio = this->maxprio - 1;
    while(prio >= 0 && ACTIVE_RUN_QUEUE[prio].empty()){
        prio--;
    }
    if(prio < 0){   // Active queue is empty, swap active and expired queue
        std::queue<Process*> *tmp;
        tmp = ACTIVE_RUN_QUEUE;
        ACTIVE_RUN_QUEUE = EXPIRED_RUN_QUEUE;
        EXPIRED_RUN_QUEUE = tmp;
        tmp = nullptr;
        prio = this->maxprio - 1;
        while(prio >= 0 && ACTIVE_RUN_QUEUE[prio].empty()){
            prio--;
        }
        if(prio < 0) return nullptr;    // expired queue is also empty
    }
    Process* proc = ACTIVE_RUN_QUEUE[prio].front();
    if(proc->rem_cpuburst == 0) proc->dynamic_prio = proc->static_prio - 1;
    ACTIVE_RUN_QUEUE[prio].pop();    
    return proc;
}