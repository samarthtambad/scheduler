#include "Scheduler.h"

class SRTF : public Scheduler {
private:
    /* data */
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
}

SRTF::~SRTF(){

}

void SRTF::add_process(Process* proc){

}

Process* SRTF::get_next_process(){
    return nullptr;
}