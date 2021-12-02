#ifndef SHM_RESOURCES_H
#define SHM_RESOURCES_H

#include <sys/ipc.h>
#include <sys/shm.h>

#define FTOK_FILE_NAME "./inc/ftok.key"
#define PROCESSES_LIST_PROJECT_ID          0 
#define NUM_ARRIVED_PROCESSES_PROJECT_ID   1
#define NUM_READY_PROCESSES_PROJECT_ID     2 
#define CLK_PROJECT_ID                     3 

#define GENERATOR_FLAG  IPC_CREAT | IPC_EXCL | 0666
#define NOT_GENERATOR_FLAG 0666

typedef struct shm_res_ids
{
    int processes_list_id, num_arrived_processes_id,
        num_ready_processes_id, clk_id;
} shm_res_ids;

#endif

