#include "Scheduler.h"

class LCFS: public Scheduler{
private:
    /* data */
    std::queue<Process*> RUN_QUEUE;
public:
    LCFS();
    ~LCFS();
    void add_process(Process*);
    Process* get_next_process();
};

LCFS::LCFS(){
    this->scheduler_type = "LCFS";
    this->is_preemptive = false;
    this->quantum = 0;
    this->maxprio = 4;
}

LCFS::~LCFS(){

}

void LCFS::add_process(Process* proc){

}

Process* LCFS::get_next_process(){
    return nullptr;
}