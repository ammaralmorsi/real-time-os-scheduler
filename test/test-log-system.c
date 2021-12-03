#include <stdlib.h>

#include "../inc/process-input-information.h"
#include "../inc/log-system.h"
#include "../inc/read-input-file.h"
#include "../inc/pcb.h"

int main(void)
{
    FILE* fptr;
    process_list* plist;
    PCB* pcb_test;

    plist = read_input_file("processes.txt");
    fptr = open_log_file("test.log");
    pcb_test = (PCB*)malloc(plist->size * sizeof(PCB));

    log_str(fptr, "THIS IS THE FIRST LINE IN THE LOG FILE\n");

    log_process_list(fptr, plist);

    for(int i = 0; i < plist->size; i++)
    {
        pcb_test[i].curr_state = i%5;
        pcb_test[i].given_info.id = plist->processes[i].id;
        pcb_test[i].given_info.arriv_time = plist->processes[i].arriv_time;
        pcb_test[i].given_info.tot_run_time = plist->processes[i].tot_run_time;
        pcb_test[i].given_info.priority = plist->processes[i].priority;
        pcb_test[i].last_time_leave_cpu = i;
        pcb_test[i].remain_time = i;
        pcb_test[i].wait_time = i;
        pcb_test[i].turn_around_time = i;
        pcb_test[i].weighted_turn_around = 1.333f * i;
        log_pcb(fptr, i, &(pcb_test[i]));
    }

    close_log_file(fptr);
    free_input_resources(&plist);
    free(pcb_test);

    return 0;
}
