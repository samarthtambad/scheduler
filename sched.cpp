#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <queue>
#include <iostream>
#include <sstream>

#include "DiscreteEventSimulation.h"
#include "classes/Process.h"
#include "schedulers/Scheduler.h"

using namespace std;

static DiscreteEventSimulation simulation;
Scheduler scheduler;
bool printVerbose = false;
bool printTrace = false;
bool printEventQueue = false;
vector<long> randvals;
long rand_ofs = 0;

queue<Process*> proc_queue;

vector<string> split (const string &s, char delim) {
    vector<string> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

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

int main(int argc, char *argv[]){

    string input_file, rand_file, schedspec;
    parse_args(argc, argv, input_file, rand_file, schedspec);
    printf("printVerbose: %d,printTrace: %d,printEventQueue: %d, SchedSpec: %s\n", printVerbose, printTrace, printEventQueue, schedspec.c_str());
    printf("Input: %s, Rand: %s \n", input_file.c_str(), rand_file.c_str());

    parseRandFile(rand_file);
    // printf("%lu", randvals.size());

    // for(int i = 0; i < 20; i++){
    //     simulation.put_event(i, myrandom(1000));
    // }

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
        
        // create process and add process-create event to event queue.
        Process* p = new Process(at, tc, cb, io);
        proc_queue.push(p);
        
        // Event e;
        // e.evtProcess = &p;
        // e.evtTimeStamp = sim_time;
        // e.newstate = STATE_CREATED;
        // simulation.put_event(&e);
        // event_queue.push(&e);
        
    }

    printf("%lu", proc_queue.size());


//    Event* evt;
//    while((evt = simulation.get_event())){
//    	printf("Event %d : %d\n", evt->id, evt->timestamp);
//    	simulation.rm_event();
//    }


    return 0;
}
