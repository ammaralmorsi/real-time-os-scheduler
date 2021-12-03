#include <string.h>
#include <stdio.h>

#include "../inc/process-input-information.h"
#include "../inc/pcb.h"
#include "../inc/log-system.h"

FILE* open_log_file(const char* file_name)
{
    return fopen(file_name, "w");
}

void close_log_file(FILE* fileptr)
{
    if(fileptr)
        fclose(fileptr);
    return;
}

void log_str(FILE* fptr, const char* str)
{
    if(fptr)
        fprintf(fptr, str);
    return;
}

void log_process_list(FILE* fptr, process_list* plist)
{
    int i;

    if(fptr && plist)
        for(i = 0; i < plist->size; i++)
            fprintf(fptr, "%d\t%d\t%d\t%d\n",
                    plist->processes[i].id,
                    plist->processes[i].arriv_time,
                    plist->processes[i].tot_run_time,
                    plist->processes[i].priority);
    return;
}

void log_pcb(FILE* fptr,int curr_time, pcb* pcbptr)
{
    if(!(fptr && pcbptr))
        return;

    char str[1024];
    char finished_str[128];
    const char* state;

    switch(pcbptr->curr_state)
    {
        case STARTED:
            state = "started";
            break;
        case RESUMED:
            state = "resumed";
            break;
        case STOPPED:
            state = "stopped";
            break;
        case FINISHED:
            state = "finished";
            break;
        default:
            state = "none";
            break;
    }

    sprintf(str, "At time %d "
                 "process %d %s "
                 "arr %d "
                 "total %d "
                 "remain %d "
                 "wait %d ",
                 curr_time,
                 pcbptr->given_info.id,
                 state,
                 pcbptr->given_info.arriv_time,
                 pcbptr->given_info.tot_run_time,
                 pcbptr->remain_time,
                 pcbptr->wait_time);

    if(pcbptr->curr_state == FINISHED)
       sprintf(finished_str, "TA %d "
                             "WTA %.2f",
                             pcbptr->turn_around_time,
                             pcbptr->weighted_turn_around);
    else
        strcpy(finished_str, "");

    strcat(str, finished_str);
    strcat(str, "\n");

    fprintf(fptr, str);

    return;
}
    


