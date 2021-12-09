#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "../inc/shm.h"
#include "../inc/clk-shm.h"
#include "../inc/schd-proc-shm.h"

void proc_set_shm_keys(void);
void proc_create_shm(void);
void proc_attach_shm(void);
void proc_free_shm(void);
void proc_stop(int);
void proc_continue(int);
void proc_exit(int);


int cpu_time;
int total_run_time;
int enter_cpu_time;
pid_t schd_id;
int arrived_signal;

int main(void)
{
    signal(SIGUSR1, proc_continue);
    signal(SIGALRM, proc_stop);
    signal(SIGINT, proc_exit);

    schd_id = getppid();

    proc_set_shm_keys();
    proc_create_shm();
    proc_attach_shm();

    cpu_time = 0;
    total_run_time = *shm_schd_running_process_remin_time;
    enter_cpu_time = *shm_clk_curr_time;
    raise(SIGALRM);
    printf("my id %d and my remain time %d\n", getpid(), total_run_time - cpu_time);

    while(cpu_time <  total_run_time)
    {
        if(arrived_signal == SIGALRM)
        {
            printf("proc stop\n");
            pause();
        }
        cpu_time = (*shm_clk_curr_time - enter_cpu_time);
    }

    proc_free_shm();
    kill(schd_id, SIGUSR2);
    return 0;
}

void proc_exit(int signum)
{
    proc_free_shm();
    exit(EXIT_SUCCESS);
}

void proc_set_shm_keys(void)
{
    ftkey_clk_curr_time = ftok(FTOK_FILE, ftk_prjid_clk_curr_time);
    ftkey_schd_running_process_remin_time = ftok(FTOK_FILE,
                                            ftk_prjid_schd_running_process_remin_time);
    return;
}

void proc_create_shm(void)
{
    shmid_clk_curr_time = shmget(ftkey_clk_curr_time, sizeof(int), SHM_NOT_CREATOR_FLAG);
    shmid_schd_running_process_remin_time = shmget(ftkey_schd_running_process_remin_time,
                                                sizeof(int), SHM_NOT_CREATOR_FLAG);
    return;
}

void proc_attach_shm(void)
{
    shm_clk_curr_time = shmat(shmid_clk_curr_time, NULL, 0);
    shm_schd_running_process_remin_time = shmat(shmid_schd_running_process_remin_time,NULL, 0);

    return;
}

void proc_free_shm(void)
{
    shmdt(shm_clk_curr_time);
    shmdt(shm_schd_running_process_remin_time);

    return;
}

void proc_stop(int signum)
{
    arrived_signal = signum;
    *shm_schd_running_process_remin_time = total_run_time - cpu_time;
    kill(schd_id, SIGUSR1);
}

void proc_continue(int signum)
{
    arrived_signal = signum;
    enter_cpu_time = *shm_clk_curr_time;
    total_run_time = *shm_schd_running_process_remin_time;
}

