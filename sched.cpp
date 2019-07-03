#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <queue>

#include "DiscreteEventSimulation.h"

using namespace std;

static DiscreteEventSimulation simulation;


int main(int argc, char *argv[]){

    simulation.put_event(1, 76);
    simulation.put_event(2, 10);
    simulation.put_event(3, 15);
    simulation.put_event(4, 18);
    simulation.put_event(6, 53);
    simulation.put_event(5, 40);

    // printf("%d", simulation.get_event()->id);

   Event* evt;
   while((evt = simulation.get_event())){
   	printf("Event %d : %d\n", evt->id, evt->timestamp);
   	simulation.rm_event();
   }


    return 0;
}
