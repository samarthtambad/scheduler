#ifndef GLOBALS
#define GLOBALS

typedef int stime_t;
// typedef int pstate;

// states of process
typedef enum {
    STATE_CREATED,  // 0
    STATE_READY,    // 1
    STATE_RUNNING,  // 2
    STATE_BLOCKED   // 3
} process_state_t;

// event transitions
typedef enum {
    TRANS_TO_READY,     //0
    TRANS_TO_RUN,       //1
    TRANS_TO_BLOCK,     //2
    TRANS_TO_PREEMPT    //3
} state_transition_t;

#endif