#ifndef SCHD_PROC_SHM_H
#define SCHD_PROC_SHM_H

#include <sys/types.h>

int     ftk_prjid_schd_running_process_remin_time;
key_t   ftkey_schd_running_process_remin_time;
int     shmid_schd_running_process_remin_time;
int*    shm_schd_running_process_remin_time;

#endif

