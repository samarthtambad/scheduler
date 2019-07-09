#include <queue>
#include "Scheduler.h"

class PriorityScheduler : public Scheduler {
private:
    /* data */
    std::queue<Process*> *ACTIVE_RUN_QUEUE;
    std::queue<Process*> *EXPIRED_RUN_QUEUE;
public:
    PriorityScheduler(stime_t);
    PriorityScheduler(stime_t, int);
    ~PriorityScheduler();
    void add_process(Process*);
    Process* get_next_process();
};

PriorityScheduler::PriorityScheduler(stime_t quantum){
    this->scheduler_type = "PRIO " + std::to_string(quantum);
    this->is_preemptive = false;
    this->quantum = quantum;
    this->maxprio = 4;
    ACTIVE_RUN_QUEUE = new std::queue<Process*>[this->maxprio];
    EXPIRED_RUN_QUEUE = new std::queue<Process*>[this->maxprio];
}

PriorityScheduler::PriorityScheduler(stime_t quantum, int maxprio){
    this->scheduler_type = "PRIO " + std::to_string(quantum);
    this->is_preemptive = false;
    this->quantum = quantum;
    this->maxprio = maxprio;
    ACTIVE_RUN_QUEUE = new std::queue<Process*>[this->maxprio];
    EXPIRED_RUN_QUEUE = new std::queue<Process*>[this->maxprio];
}

PriorityScheduler::~PriorityScheduler(){
    
}

void PriorityScheduler::add_process(Process* proc){
    if(proc->rem_cpuburst > 0){ // was preempted
        proc->dynamic_prio--;
        if(proc->dynamic_prio == -1) {
            proc->dynamic_prio = proc->static_prio - 1;
            EXPIRED_RUN_QUEUE[proc->dynamic_prio].push(proc);
            // printf("PROC %d ADDED TO EXPIRED QUEUE\n", proc->pid);
        } else {
            ACTIVE_RUN_QUEUE[proc->dynamic_prio].push(proc);
            // printf("PROC %d ADDED TO ACTIVE QUEUE\n", proc->pid);
        }
    } else if(proc->rem_cpuburst == 0 && (proc->totaltime - proc->rem) == 0) {   // was created
        ACTIVE_RUN_QUEUE[proc->dynamic_prio].push(proc);
        // printf("PROC %d ADDED TO ACTIVE QUEUE AFTER CREATION\n", proc->pid);
    } else {    // was blocked
        proc->dynamic_prio = proc->static_prio - 1;
        ACTIVE_RUN_QUEUE[proc->dynamic_prio].push(proc);
        // printf("PROC %d ADDED TO EXPIRED QUEUE AFTER BLOCK\n", proc->pid);
    }
}

Process* PriorityScheduler::get_next_process(){
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
        // printf("ACTIVE QUEUE <-> EXPIRED QUEUE\n");
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