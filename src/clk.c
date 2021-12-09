#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "../inc/shm.h"
#include "../inc/clk-shm.h"

void clk_set_shm_keys(void);
void clk_create_shm(void);
void clk_attach_shm(void);
void clk_free_shm(void);
void clk_exit(int);

int main(void)
{
    signal(SIGINT, clk_exit);

    clk_set_shm_keys();
    clk_create_shm();
    clk_attach_shm();

    while(1)
    {
        (*shm_clk_curr_time)++;
        sleep(1);
    }

    return 0;
}

void clk_exit(int signum)
{
    clk_free_shm();
    exit(EXIT_SUCCESS);
}

void clk_set_shm_keys(void)
{
    ftkey_clk_curr_time = ftok(FTOK_FILE, ftk_prjid_clk_curr_time);
    return;
}

void clk_create_shm(void)
{
    shmid_clk_curr_time = shmget(ftkey_clk_curr_time, sizeof(int), SHM_NOT_CREATOR_FLAG);
    return;
}

void clk_attach_shm(void)
{
    shm_clk_curr_time = shmat(shmid_clk_curr_time, NULL, 0);
    return;
}

void clk_free_shm(void)
{
    shmdt(shm_clk_curr_time);
    return;
}

