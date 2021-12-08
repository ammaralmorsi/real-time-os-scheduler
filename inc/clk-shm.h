#ifndef CLK_SHM_H
#define CLK_SHM_H

#include <sys/types.h>

int     ftk_prjid_clk_curr_time = 0;
key_t   ftkey_clk_curr_time = 0;
int     shmid_clk_curr_time = 0;
int*    shm_clk_curr_time = 0;

#endif

