#include <string>
#include "../classes/Process.h"

#ifndef SCHEDULER_H
#define SCHEDULER_H

class Scheduler {

private:
    /* data */
public:

    string scheduler_type;
    bool is_preemptive;
    stime_t quantum;
    int maxprio;

    Scheduler();
    ~Scheduler();
    virtual void add_process(Process*) =0;
    virtual Process* get_next_process() =0;
    void print_info();
};

Scheduler::Scheduler() {

}

Scheduler::~Scheduler() {

}

void Scheduler::print_info(){
    printf("%s \n", scheduler_type.c_str());
}

#endif