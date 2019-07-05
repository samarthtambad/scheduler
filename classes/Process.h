
#include "../misc/globals.h"

class Process {
private:
    /* data */
public:
    int pid;
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

    Process(stime_t at, stime_t tc, stime_t cb, stime_t io);
    ~Process();
};

Process::Process(stime_t at, stime_t tc, stime_t cb, stime_t io) {
    this->at = at;
    this->tc = tc;
    this->cb = cb;
    this->io = io;
    this->state = STATE_CREATED;
}

Process::~Process() {

}
