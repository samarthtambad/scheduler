
#include "../misc/globals.h"

#ifndef PROCESS_H
#define PROCESS_H
class Process {
// private:
public:
    /* data */
    int pid;
    int static_priority;
    int dynamic_priority;
    process_state_t state;
    stime_t at;             // arrival time
    stime_t tc;             // total cpu time
    stime_t cb;             // cpu burst
    stime_t io;             // io burst
    stime_t ft;             // finishing time
    stime_t tt;             // turnaround time = (finishing time - arrival time)
    stime_t it;             // io time (i.e. time in blocked state)
    stime_t cw;             // cpu waiting time (i.e. time in ready state)
    stime_t state_ts;       // timestamp of the last transition
    stime_t rem;            // time remaining for completing this job

    Process(int, int, stime_t, stime_t, stime_t, stime_t);
    ~Process();
};

Process::Process(int pid, int static_priority, stime_t at, stime_t tc, stime_t cb, stime_t io) {
    this->pid = pid;
    this->at = at;
    this->tc = tc;
    this->cb = cb;
    this->io = io;
    this->state = STATE_CREATED;
    this->state_ts = 0;
    this->ft = 0;
    this->tt = 0;
    this->it = 0;
    this->cw = 0;
    this->rem = tc;
    this->static_priority = static_priority;
    this->dynamic_priority = static_priority - 1;
}

Process::~Process() {

}

#endif