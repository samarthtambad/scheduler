#include <stack>
#include "Scheduler.h"

class LCFS: public Scheduler{
private:
    /* data */
    std::stack<Process*> RUN_QUEUE;
public:
    LCFS();
    ~LCFS();
    void add_process(Process*);
    Process* get_next_process();
};

LCFS::LCFS(){
    this->scheduler_type = "LCFS";
    this->scheduler_symbol = 'L';
    this->is_preemptive = false;
    this->quantum = 10000;
    this->maxprio = 4;
}

LCFS::~LCFS(){

}

void LCFS::add_process(Process* proc){
    RUN_QUEUE.push(proc);
}

Process* LCFS::get_next_process(){
    if(RUN_QUEUE.empty()) return nullptr;
    Process* proc = RUN_QUEUE.top();
    RUN_QUEUE.pop();    
    return proc;
}