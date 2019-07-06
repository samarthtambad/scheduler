
#include <queue>
#include "../classes/Process.h"

class Scheduler {

private:
    /* data */
public:

    bool isPreemptive;
    stime_t quantum;
    int maxprio;

    Scheduler(/* args */);
    ~Scheduler();
    virtual void add_process(Process*) =0;
    virtual Process* get_next_process() =0;
};

Scheduler::Scheduler(/* args */) {

}

Scheduler::~Scheduler() {

}