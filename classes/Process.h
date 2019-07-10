
#include "../misc/globals.h"

#ifndef PROCESS_H
#define PROCESS_H

class Process {

public:

    int pid;
    int static_prio;
    int dynamic_prio;
    process_state_t state;
    stime_t arrival;             // arrival time
    stime_t totaltime;           // total execution (cpu) time
    stime_t cpuburst;            // cpu burst
    stime_t ioburst;             // io burst
    stime_t state_ts;            // timestamp of the last transition
    stime_t iowaittime;          // io time (i.e. time in blocked state)
    stime_t cpuwaittime;         // cpu waiting time (i.e. time in ready state)
    stime_t rem;                 // time remaining for completing this job
    
    stime_t current_cpuburst;
    stime_t rem_cpuburst;

    Process(int, int, stime_t, stime_t, stime_t, stime_t);
    ~Process();
    void print_info();
};

Process::Process(int pid, int static_prio, stime_t arrival, stime_t totaltime, stime_t cpuburst, stime_t ioburst) {
    this->pid = pid;
    this->arrival = arrival;
    this->totaltime = totaltime;
    this->cpuburst = cpuburst;
    this->ioburst = ioburst;
    this->state = STATE_CREATED;
    this->state_ts = arrival;
    this->iowaittime = 0;
    this->cpuwaittime = 0;
    this->rem = totaltime;
    this->static_prio = static_prio;
    this->dynamic_prio = static_prio - 1;

    this->current_cpuburst = 0;
    this->rem_cpuburst = 0;
}

Process::~Process() {

}

void Process::print_info(){
    printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n", 
        pid, 
        arrival, totaltime, cpuburst, ioburst, static_prio, 
        state_ts, 
        state_ts - arrival, 
        iowaittime, 
        cpuwaittime);
}

#endif