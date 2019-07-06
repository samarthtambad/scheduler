
#include "Scheduler.h"

class FCFS : public Scheduler {
private:
    /* data */
    std::queue<Process*> RUN_QUEUE;
public:
    FCFS(/* args */);
    ~FCFS();
    void add_process(Process*);
    Process* get_next_process();
};

FCFS::FCFS(/* args */) {
    isPreemptive = false;
    quantum = 0;
    maxprio = 4;
}

FCFS::~FCFS() {

}

void FCFS::add_process(Process* proc) {
    RUN_QUEUE.push(proc);
}

Process* FCFS::get_next_process() {
    if(RUN_QUEUE.empty()) return nullptr;
    Process* proc = RUN_QUEUE.front();
    RUN_QUEUE.pop();    
    return proc;
}