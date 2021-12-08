#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <signal.h>


#include "../inc/log-system.h"
#include "../inc/read-input-file.h"
#include "../inc/algorithm-type.h"
#include "../inc/get-algorithm.h"
#include "../inc/shm.h"
#include "../inc/clk-shm.h"
#include "../inc/gen-schd-shm.h"

/*  functions prototypes */
void generator_set_shm_ids(void);
void generator_create_shm(void);
void generator_attach_shm(void);
int  simulation_finished(void);
int no_more_process_to_arrive(void);
int no_more_process_to_schedule(void);
void generator_exit(int);
void generator_free_shm(void);
void generator_free_resources(void);
/*************************/

/*  global variables     */
process_list* plist;
algo_type     algtyp;
pid_t         schd_id;
pid_t         clk_id;
int           index_of_process_to_arrive_next;
const int TIME_QUANTUM = 3; /*sec*/
/*************************/

int main(void)
{
    int num_arrived;
    int time_to_arriv;

    signal(SIGINT, generator_exit);

    plist = read_input_file("processes.txt");
    algtyp = get_algorithm_type();

    generator_set_shm_ids();
    generator_create_shm();
    generator_attach_shm();

    switch(algtyp)
    {
        case HPF:
            *shm_gen_alarm_time_out = INT_MAX; /* alarm has no meaning for HPF algorithm */
            break;
        case RR:
            *shm_gen_alarm_time_out = TIME_QUANTUM;
            break;
        case SRTN:
            break; /* timeout will be set inside the main loop */
        default:
            exit(EXIT_FAILURE);
            break;
    }

    /*fork the schedular*/
    /*stop untill the schedular attach to memory*/
    /*fork the clock*/

    index_of_process_to_arrive_next = 0;
    /* main loop */
    while(!simulation_finished())
    {
        if(no_more_process_to_schedule())
        {
            time_to_arriv = 
                plist->processes[index_of_process_to_arrive_next].arriv_time -
                     *shm_clk_curr_time;

            if(time_to_arriv > 0)
                sleep(time_to_arriv);
        }

        num_arrived = 0;
        while(plist->processes[index_of_process_to_arrive_next].arriv_time <= 
                *shm_clk_curr_time)
            shm_gen_processes_arrived[num_arrived++] = 
                plist->processes[index_of_process_to_arrive_next++];

        *shm_gen_num_arrived_processes = num_arrived;

        if(algtyp == SRTN)
            *shm_gen_alarm_time_out = 
                plist->processes[index_of_process_to_arrive_next].arriv_time -
                *shm_clk_curr_time;

        
        /*kill(schd_id, SIGINT);*/
        pause();
    }

    generator_free_resources();

    return 0;
}

int simulation_finished(void)
{
    if(no_more_process_to_arrive() && no_more_process_to_schedule())
        return 1;

    return 0;
}

void generator_set_shm_ids(void)
{
    ftkey_clk_curr_time = ftok(FTOK_FILE, ftk_prjid_clk_curr_time);

    ftkey_gen_num_arrived_processes = ftok(FTOK_FILE, ftk_prjid_gen_num_arrived_processes);

    ftkey_gen_processes_arrived = ftok(FTOK_FILE, ftk_prjid_gen_processes_arrived);

    ftkey_gen_alarm_time_out = ftok(FTOK_FILE, ftk_prjid_gen_alarm_time_out);

    ftkey_schd_num_ready_processes = ftok(FTOK_FILE, ftk_prjid_schd_num_ready_processes);

    return;
}

void generator_create_shm(void)
{
    shmid_clk_curr_time = shmget(ftkey_clk_curr_time, 
                                 sizeof(int), 
                                 SHM_CREATOR_FLAG);

    shmid_gen_num_arrived_processes = shmget(ftkey_gen_num_arrived_processes,
                                            sizeof(int), 
                                             SHM_CREATOR_FLAG);

    shmid_gen_processes_arrived = shmget(ftkey_gen_processes_arrived,
                                         plist->size * sizeof(piinfo), 
                                         SHM_CREATOR_FLAG);

    shmid_gen_alarm_time_out = shmget(ftkey_gen_alarm_time_out, 
                                      sizeof(int), 
                                      SHM_CREATOR_FLAG);

    shmid_schd_num_ready_processes = shmget(ftkey_schd_num_ready_processes,
                                            sizeof(int), 
                                            SHM_CREATOR_FLAG);
    
    return;
}

void generator_attach_shm(void)
{
    shm_clk_curr_time = shmat(shmid_clk_curr_time, NULL, 0);

    shm_gen_num_arrived_processes = shmat(shmid_gen_num_arrived_processes, NULL, 0);

    shm_gen_processes_arrived = shmat(shmid_gen_processes_arrived, NULL, 0);

    shm_gen_alarm_time_out = shmat(shmid_gen_alarm_time_out, NULL, 0);

    shm_schd_num_ready_processes = shmat(shmid_schd_num_ready_processes, NULL, 0);

    return;
}

void generator_free_shm(void)
{
    shmdt(shm_clk_curr_time);
    shmctl(shmid_clk_curr_time, IPC_RMID, 0);

    shmdt(shm_gen_num_arrived_processes);
    shmctl(shmid_gen_num_arrived_processes, IPC_RMID, 0);

    shmdt(shm_gen_processes_arrived);
    shmctl(shmid_gen_processes_arrived, IPC_RMID, 0);

    shmdt(shm_gen_alarm_time_out);
    shmctl(shmid_gen_alarm_time_out, IPC_RMID, 0);

    shmdt(shm_schd_num_ready_processes);
    shmctl(shmid_schd_num_ready_processes, IPC_RMID, 0);

    return;
}

void generator_exit(int signum)
{
    generator_free_resources();
    exit(EXIT_SUCCESS);
}

void generator_free_resources(void)
{
    free_input_resources(&plist);
    generator_free_shm();

    return;
}

int no_more_process_to_arrive(void)
{
    return index_of_process_to_arrive_next >= plist->size;
}

int no_more_process_to_schedule(void)
{
    return *shm_schd_num_ready_processes == 0;
}

