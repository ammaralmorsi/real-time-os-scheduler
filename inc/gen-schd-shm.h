#ifndef GEN_SCHD_SHM_H
#define GEN_SCHD_SHM_H

#include "process-input-information.h"

#include <sys/types.h>

int     ftk_prjid_gen_num_arrived_processes = 2;
key_t   ftkey_gen_num_arrived_processes;
int     shmid_gen_num_arrived_processes;
int*    shm_gen_num_arrived_processes;

int     ftk_prjid_gen_processes_arrived = 3;
key_t   ftkey_gen_processes_arrived;
int     shmid_gen_processes_arrived;
piinfo* shm_gen_processes_arrived;

int     ftk_prjid_gen_alarm_time_out = 4;
key_t   ftkey_gen_alarm_time_out;
int     shmid_gen_alarm_time_out;
int*    shm_gen_alarm_time_out;

int     ftk_prjid_schd_num_ready_processes = 5;
key_t   ftkey_schd_num_ready_processes;
int     shmid_schd_num_ready_processes;
int*    shm_schd_num_ready_processes;

#endif

