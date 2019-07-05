
#include <queue>
#include "../classes/Process.h"

class Scheduler {

private:
    /* data */
    std::queue<Process*> RUN_QUEUE;
public:
    Scheduler(/* args */);
    ~Scheduler();
    virtual void add_process(Process*);
    virtual Process* get_next_process();
};

Scheduler::Scheduler(/* args */) {

}

Scheduler::~Scheduler() {

}

void Scheduler::add_process(Process* proc) {
    RUN_QUEUE.push(proc);
}