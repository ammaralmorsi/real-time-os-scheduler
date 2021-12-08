#ifndef PROCESS_INPUT_INFORMATION_H
#define PROCESS_INPUT_INFORMATION_H

typedef struct process_input_information
{
    int id, arriv_time, tot_run_time, priority; 
} piinfo;

typedef struct process_list
{
    int size;
    piinfo* processes;
} process_list;

#endif

