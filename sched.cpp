#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <queue>
#include <iostream>
#include <sstream>

#include "DiscreteEventSimulation.h"
#include "schedulers/FCFS.h"
// #include "schedulers/LCFS.h"
// #include "schedulers/SRTF.h"
// #include "schedulers/RoundRobin.h"
// #include "schedulers/PriorityScheduler.h"
// #include "schedulers/PreemptivePriorityScheduler.h"

using namespace std;

static DiscreteEventSimulation simulation;
Scheduler* scheduler;

// char sched_sym;
// stime_t quantum;
// int maxprio;

bool printVerbose = false;
bool printTrace = false;
bool printEventQueue = false;
vector<long> randvals;
long rand_ofs = 0;

stime_t CURRENT_TIME = 0;
bool CALL_SCHEDULER = true;
Process* CURRENT_RUNNING_PROCESS = nullptr;
queue<Process*> proc_list;

stime_t last_ftime;
double cpu_util, io_util, avg_turnaround, avg_cpu_wt, throughput;


void parse_args(int argc, char *argv[], string &input_file, string &rand_file, string &schedspec){
    int c;
    while ((c = getopt (argc, argv, "vtes:")) != -1){
        switch(c){
            case 'v': 
                printVerbose = true;
                break;
            case 't':
                printTrace = true;
                break;
            case 'e':
                printEventQueue = true;
                break;
            case 's':
                schedspec = optarg;
                char sched_sym;
                stime_t quantum;
                int maxprio;
                sched_sym = optarg[0];
                switch (sched_sym){
                    case 'F':   // FCFS
                        // scheduler = (Scheduler *) new FCFS();
                        printf("%c", sched_sym);
                        break;
                    case 'L':   // LCFS
                        printf("%c", sched_sym);
                        break;
                    case 'S':   // SRTF
                        printf("%c", sched_sym);
                        break;
                    case 'R':   // R<num>
                        quantum = atoi(optarg+1);
                        printf("%c<%d>", sched_sym, quantum);
                        break;
                    case 'P':   // P<num>[:<maxprio>]
                        char *str, *res;
                        str = optarg+1;
                        res = strtok(str, ":");
                        quantum = atoi(res);
                        res = strtok(NULL,":");
                        if(res == NULL){
                            maxprio = 0;    // default
                        } else {
                            maxprio = atoi(res);
                        }
                        printf("%c<%d>:%d", sched_sym, quantum, maxprio);
                        break;
                    case 'E':   // E<num>[:<maxprios>]
                        char *str1, *res1;
                        str1 = optarg+1;
                        res1 = strtok(str1, ":");
                        quantum = atoi(res1);
                        res1 = strtok(NULL,":");
                        if(res1 == NULL){
                            maxprio = 0;    // default
                        } else {
                            maxprio = atoi(res1);
                        }
                        printf("%c<%d>:%d", sched_sym, quantum, maxprio);
                        break;
                    default:
                        break;
                }
                break;
            case '?':
                if (optopt == 's')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
            default:
                abort();
        }
    }

    int idx = optind;
    if(idx < argc) input_file = argv[idx];
    idx++;
    if(idx < argc) rand_file = argv[idx];
}

void parseRandFile(string rand_file){
    FILE *fptr;
    fptr = fopen(rand_file.c_str(), "r");
    if(fptr == NULL){
        printf("cannot open file");
    }
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    char *ptr;
    char delim[] = " \n\t";

    linelen = getline(&line, &linecap, fptr);
    ptr = strtok(line, delim);
    int num_rand = atoi(ptr);
    for(int i = 0; i < num_rand; i++){
        linelen = getline(&line, &linecap, fptr);
        ptr = strtok(line, delim);
        long num = atoi(ptr);
        randvals.push_back(num);
    }
}

int myrandom(int burst){
    int res = 1 + (randvals[rand_ofs] % burst);
    rand_ofs++;
    if(rand_ofs == randvals.size()) rand_ofs = 0;
    return res;
}


void start_simulation(){
    Event* evt;
    while((evt = simulation.get_event()) != nullptr){
        Process* proc = evt->evtProcess;
        CURRENT_TIME = evt->evtTimeStamp;
        stime_t timeInPrevState = CURRENT_TIME - proc->state_ts;

        // printf("%d events in queue \n", simulation.get_num_of_events());
        
        switch (evt->transition) {
            case TRANS_TO_READY:
                // must come from BLOCKED or PREEMPTION
                // must add to run queue
                if(evt->old_state == STATE_CREATED){
                    if(printVerbose) printf("%d %d %d: CREATED -> READY \n", CURRENT_TIME, proc->pid, timeInPrevState);
                } else if(evt->old_state == STATE_BLOCKED){
                    if(printVerbose) printf("%d %d %d: BLOCK -> READY \n", CURRENT_TIME, proc->pid, timeInPrevState);
                    //------- something pending here ----------//
                }
                scheduler->add_process(proc);
                CALL_SCHEDULER = true;
                break;
            case TRANS_TO_RUN:
                // create event for either preemption or blocking
                proc->cpuwaittime = proc->cpuwaittime + timeInPrevState;
                if(scheduler->is_preemptive && proc->cpuburst > scheduler->quantum){   // is preemptive and was preempted
                    simulation.put_event(new Event(proc, (CURRENT_TIME + scheduler->quantum), STATE_RUNNING, STATE_BLOCKED, TRANS_TO_PREEMPT));
                    if(printVerbose) printf("READY -> RUNNG from preemption \n");
                } else {    // non-preemptive
                    int cb;
                    cb = myrandom(proc->cpuburst);
                    cb = (proc->rem < cb)? proc->rem : cb;
                    simulation.put_event(new Event(proc, (CURRENT_TIME + cb), STATE_RUNNING, STATE_BLOCKED, TRANS_TO_BLOCK));
                    if(printVerbose) printf("%d %d %d: READY -> RUNNG cb=%d rem=%d prio=%d \n", CURRENT_TIME, proc->pid, timeInPrevState, cb, proc->rem, proc->dynamic_prio);
                }
                break;
            case TRANS_TO_BLOCK:
                // create an event for when process becomes ready again
                int io;
                proc->rem = proc->rem - timeInPrevState;
                CURRENT_RUNNING_PROCESS = nullptr;
                if(proc->rem <= 0){
                    if(printVerbose) printf("%d %d %d: Done \n", CURRENT_TIME, proc->pid, timeInPrevState);
                } else {
                    io = myrandom(proc->ioburst);
                    proc->iowaittime = proc->iowaittime + io;
                    simulation.put_event(new Event(proc, (CURRENT_TIME + io), STATE_BLOCKED, STATE_READY, TRANS_TO_READY));
                    if(printVerbose) printf("%d %d %d: RUNNG -> BLOCK ib=%d rem=%d \n", CURRENT_TIME, proc->pid, timeInPrevState, io, proc->rem);
                }
                CALL_SCHEDULER = true;
                break;
            case TRANS_TO_PREEMPT:
                // add to runqueue (no event is generated)
                proc->dynamic_prio--;
                if(proc->dynamic_prio == -1) proc->dynamic_prio = proc->static_prio - 1;
                scheduler->add_process(proc);
                CALL_SCHEDULER = true;
                if(printVerbose) printf("preemption \n");
                break;
            default:
                break;
        }
        delete evt;
        evt = nullptr;

        proc->state_ts = CURRENT_TIME;

        // printf("%d events in queue \n", simulation.get_num_of_events());

        if(CALL_SCHEDULER){
            // printf("SCHEDULER CALLED \n");
            if(simulation.get_next_event_time() == CURRENT_TIME){
                // printf("get_next_event_time() = %d, CURRENT_TIME = %d \n", simulation.get_next_event_time(), CURRENT_TIME);
                continue;   // process next event from Event queue
            }
            CALL_SCHEDULER = false;     // reset global flag
            if(CURRENT_RUNNING_PROCESS == nullptr){
                // printf("CURRENT_RUNNING_PROCESS = nullptr \n");
                CURRENT_RUNNING_PROCESS = scheduler->get_next_process();
                if(CURRENT_RUNNING_PROCESS == nullptr){
                    // printf("No processes to run \n");
                    continue;
                }
                // create event to make this process runnable for some time.
                // printf("Ready to running event added \n");
                simulation.put_event(new Event(CURRENT_RUNNING_PROCESS, (CURRENT_TIME), STATE_READY, STATE_RUNNING, TRANS_TO_RUN));
            }
            // printf("CURRENT_RUNNING_PROCESS = %d \n", CURRENT_RUNNING_PROCESS->pid);
        }
    }
}

void print_results(){
    scheduler->print_info();
    while(!proc_list.empty()){
        Process* p = proc_list.front();
        proc_list.pop();
        p->print_info();
    }
    printf("SUM: \t %d %.2lf %.2lf %.2lf %.2lf %.3lf", 
    last_ftime, 
    cpu_util, 
    io_util, 
    avg_turnaround, 
    avg_cpu_wt, 
    throughput);
}

int main(int argc, char *argv[]){

    string input_file, rand_file, schedspec;
    parse_args(argc, argv, input_file, rand_file, schedspec);
    printf("printVerbose: %d,printTrace: %d,printEventQueue: %d, SchedSpec: %s\n", printVerbose, printTrace, printEventQueue, schedspec.c_str());
    printf("Input: %s, Rand: %s \n", input_file.c_str(), rand_file.c_str());

    parseRandFile(rand_file);

    /*
    // test by adding events at random times
    for(int i = 0; i < 20; i++){
        int time;
        if(i%3 == 0) time = 10;
        else time = myrandom(1000);
        Event* e = new Event(nullptr, time, STATE_CREATED, STATE_READY, TRANS_TO_READY);
        e->id = i;
        simulation.put_event(e);
    }
    */
    
    // parse input file
    FILE *fptr;
    fptr = fopen(input_file.c_str(), "r");
    if(fptr == NULL){
        printf("cannot open file");
    }

    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    char *ptr;
    char delim[] = " \n\t";

    // for initial testing only. Later, assign scheduler based on -s argument
    scheduler = (Scheduler *) new FCFS();

    int count = 0;
    while ((linelen = getline(&line, &linecap, fptr)) > 0){
        int at, tc, cb, io;
        ptr = strtok(line, delim);  // AT
        at = atoi(ptr);
        ptr = strtok(NULL, delim);  // TC
        tc = atoi(ptr);
        ptr = strtok(NULL, delim);  // CB
        cb = atoi(ptr);
        ptr = strtok(NULL, delim);  // IO
        io = atoi(ptr);
        printf("AT: %d, TC: %d, CB: %d, IO: %d \n", at, tc, cb, io);
        
        // create process and add created->ready event to event queue.
        Process* p = new Process(count, myrandom(scheduler->maxprio), at, tc, cb, io);
        count++;
        proc_list.push(p);
        Event* e = new Event(p, at, STATE_CREATED, STATE_READY, TRANS_TO_READY);
        simulation.put_event(e);
        
    }

//     Process* p;
//     while((p = scheduler->get_next_process()) != nullptr){
//         printf("AT: %d, TC: %d, CB: %d, IO: %d \n", p->at, p->tc, p->cb, p->io);
//     }

//    Event* evt;
//    while((evt = simulation.get_event()) != nullptr){
//    	printf("Event %d : %d\n", evt->count, evt->evtTimeStamp);
//    }

    start_simulation();

    print_results();

    return 0;
}
