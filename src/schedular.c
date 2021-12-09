#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>


#include "../inc/log-system.h"
#include "../inc/algorithm-type.h"
#include "../inc/shm.h"
#include "../inc/clk-shm.h"
#include "../inc/gen-schd-shm.h"
#include "../inc/schd-proc-shm.h"
#include "../inc/pcb.h"
#include "../inc/min-heap.h"
#include "../inc/pcb-queue.h"

/* functions prototypes */
void schedular_set_shm_keys(void);
void schedular_create_shm(void);
void schedular_attach_shm(void);
void schedular_free_shm(void);
void schedular_exit(int);
void schedular_free_resources(void);
void run_assigned_process(void);
void add_arrived_processes(void);
void remove_finished_process(void);
void pause_running_process(void);
void schedular_context_switch(int);
void handler(int);
/*************************/

/* global variables      */
pcb_queue queue;
pcb_heap  heap;
PCB current_running_process;
algo_type algtyp;
pid_t gen_id;
int max_num_processes;
int arrived_signal;
FILE* file;
/*************************/

void handler(int signum)
{
    arrived_signal = signum;
}

int main(int argc, char** argv)
{
    signal(SIGINT, schedular_exit);
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);
    signal(SIGALRM, handler);

    gen_id = getppid();

    if(argc < 2)
        exit(EXIT_FAILURE);

    file = open_log_file("schedular.log");

    sscanf(argv[1], "%d", &algtyp);

    schedular_set_shm_keys();
    schedular_create_shm();
    schedular_attach_shm();

    switch(algtyp)
    {
        case RR:
            queue = create_pcb_queue(*shm_gen_num_arrived_processes);
            break;
        case HPF:
        case SRTN:
            heap = create_pcb_heap(*shm_gen_num_arrived_processes);
            break;
        default:
            raise(SIGINT);
    }


    *shm_schd_num_ready_processes = 0;
    kill(gen_id, SIGCONT);

    while(1)
    {
        printf("number of process in the queue%d\n", *shm_schd_num_ready_processes);
        pause();
        schedular_context_switch(arrived_signal);
        run_assigned_process();
        alarm(*shm_gen_alarm_time_out);
    }

   return 0;
}

void schedular_context_switch(int signum)
{
    alarm(0);
    printf("context\n");

    if(signum == SIGUSR1)
        add_arrived_processes();
    else
    {
        if(signum == SIGUSR2)
            remove_finished_process();
        else /*alarm timeout*/
            pause_running_process();
        printf("continue generator\n");
        kill(gen_id, SIGCONT);
        printf("wait for generator signal...\n");
        pause();
        printf("signal arrived %d\n", arrived_signal);
        schedular_context_switch(arrived_signal);
    }
    return;
}

void add_arrived_processes(void)
{
    int i;
    PCB pcb;
    printf("add num arrived %d\n", *shm_gen_num_arrived_processes);

    for(i = 0; i < *shm_gen_num_arrived_processes; i++)
    {
        pcb.curr_state = NONE_PS;
        pcb.given_info = shm_gen_processes_arrived[i];
        pcb.last_time_leave_cpu = pcb.given_info.arriv_time;
        pcb.remain_time = pcb.given_info.tot_run_time;
        pcb.wait_time = 0;
        pcb.turn_around_time = 0;
        pcb.weighted_turn_around = 0.0f;
        *shm_schd_running_process_remin_time = pcb.remain_time;

        if((pcb.sys_id = fork()) == 0)
            execl("./process.out", "process.out",NULL);

        printf("created process id %d\n", pcb.sys_id);
        printf("fork process\n");
        pause();
        printf("process wakeme up\n");

        if(algtyp == RR)
            enqueue_pcb(&queue, pcb);
        else
        {
            if(algtyp == SRTN)
                pcb.key = pcb.remain_time;
            else/*HPF*/
                pcb.key = pcb.given_info.priority;

            insert_pcb_heap(&heap, pcb);
        }

        (*shm_schd_num_ready_processes)++;
    }

    return;
}

void remove_finished_process(void)
{
    printf("remove\n");
    printf("to remove id %d\n", current_running_process.sys_id);

    wait(NULL);

    if(algtyp == RR)
        dequeue_pcb(&queue);
    else
        extract_min_pcb_heap(&heap);

    current_running_process.turn_around_time = *shm_clk_curr_time - 
                                               current_running_process.given_info.arriv_time;
    current_running_process.curr_state = FINISHED;
    current_running_process.remain_time = 0;
    current_running_process.weighted_turn_around = current_running_process.turn_around_time / 
                                   (float) current_running_process.given_info.tot_run_time;
    log_pcb(file,*shm_clk_curr_time, &current_running_process);
    printf("at time %d process %d finished arrive at %d\n", *shm_clk_curr_time, current_running_process.given_info.id, current_running_process.given_info.arriv_time);

    (*shm_schd_num_ready_processes)--;
    return;
}

void run_assigned_process(void)
{
    if(algtyp == RR)
        current_running_process = get_front_pcb(&queue);
    else
        current_running_process = get_min_pcb_heap(&heap);

    if(current_running_process.curr_state == NONE_PS)
        current_running_process.curr_state = STARTED;
    else
        current_running_process.curr_state = RESUMED;

    *shm_schd_running_process_remin_time = current_running_process.remain_time;

    current_running_process.wait_time += *shm_clk_curr_time -
                                          current_running_process.last_time_leave_cpu;

    log_pcb(file,*shm_clk_curr_time, &current_running_process);

    printf("runn assigned process\n");
    printf("curring running id %d\n", current_running_process.sys_id);
    kill(current_running_process.sys_id, SIGUSR1);

    return;
}

void pause_running_process(void)
{
    printf("pause\n");
    printf("curring running id %d\n", current_running_process.sys_id);

    if(algtyp == RR)
    {
        if(!is_empty_pcb_queue(&queue))
        {
            kill(current_running_process.sys_id, SIGALRM);
            pause();
            dequeue_pcb(&queue);
            current_running_process.remain_time = *shm_schd_running_process_remin_time;
            current_running_process.last_time_leave_cpu = *shm_clk_curr_time;
            current_running_process.curr_state = STOPPED;
            enqueue_pcb(&queue, current_running_process);
        }
    }
    else
    {
        if(!is_empty_pcb_heap(&heap))
        {
            kill(current_running_process.sys_id, SIGALRM);
            pause();
            extract_min_pcb_heap(&heap);
            current_running_process.remain_time = *shm_schd_running_process_remin_time;
            if(algtyp == SRTN)
                current_running_process.key = current_running_process.remain_time;
            current_running_process.last_time_leave_cpu = *shm_clk_curr_time;
            current_running_process.curr_state = STOPPED;
            insert_pcb_heap(&heap, current_running_process);
        }
    }

    log_pcb(file, *shm_clk_curr_time, &current_running_process);

    return;
}
void schedular_exit(int signum)
{
    schedular_free_resources();
    exit(EXIT_SUCCESS);
}

void schedular_free_resources(void)
{
    close_log_file(file);

    switch(algtyp)
    {
        case RR:
            free_pcb_queue(&queue);
            break;
        case SRTN:
        case HPF:
            free_pcb_heap(&heap);
            break;
        default:
            break;
    }

    schedular_free_shm();
}

void schedular_set_shm_keys(void)
{
    ftkey_schd_running_process_remin_time = ftok(FTOK_FILE, 
                                            ftk_prjid_schd_running_process_remin_time);
    
    ftkey_schd_num_ready_processes = ftok(FTOK_FILE,
                                     ftk_prjid_schd_num_ready_processes);

    ftkey_clk_curr_time = ftok(FTOK_FILE,
                          ftk_prjid_clk_curr_time);

    ftkey_gen_alarm_time_out = ftok(FTOK_FILE,
                                ftk_prjid_gen_alarm_time_out);

    ftkey_gen_processes_arrived = ftok(FTOK_FILE,
                                    ftk_prjid_gen_processes_arrived);

    ftkey_gen_num_arrived_processes = ftok(FTOK_FILE,
                                       ftk_prjid_gen_num_arrived_processes);
    return;
}

void schedular_create_shm(void)
{
    shmid_schd_running_process_remin_time = shmget(ftkey_schd_running_process_remin_time,
                                                    sizeof(int), 
                                                    SHM_CREATOR_FLAG);

    shmid_clk_curr_time = shmget(ftkey_clk_curr_time, 
                                 sizeof(int), 
                                 SHM_NOT_CREATOR_FLAG);

    shmid_gen_alarm_time_out = shmget(ftkey_gen_alarm_time_out, 
                                      sizeof(int), 
                                      SHM_NOT_CREATOR_FLAG);

    shmid_gen_processes_arrived = shmget(ftkey_gen_processes_arrived,
                                       max_num_processes* sizeof(piinfo),
                                       SHM_NOT_CREATOR_FLAG);

    shmid_gen_num_arrived_processes = shmget(ftkey_gen_num_arrived_processes,
                                            sizeof(int),
                                            SHM_NOT_CREATOR_FLAG);

    shmid_schd_num_ready_processes = shmget(ftkey_schd_num_ready_processes,
                                            sizeof(int),
                                            SHM_NOT_CREATOR_FLAG);
    return;
}

void schedular_attach_shm(void)
{
    shm_clk_curr_time = shmat(shmid_clk_curr_time, NULL, 0);

    shm_gen_num_arrived_processes = shmat(shmid_gen_num_arrived_processes, NULL, 0);

    shm_gen_processes_arrived = shmat(shmid_gen_processes_arrived, NULL, 0);

    shm_gen_alarm_time_out = shmat(shmid_gen_alarm_time_out, NULL, 0);

    shm_schd_num_ready_processes = shmat(shmid_schd_num_ready_processes, NULL, 0);

    shm_schd_running_process_remin_time = shmat(shmid_schd_running_process_remin_time,
                                                NULL, 0);
    return;
}

void schedular_free_shm(void)
{
    shmdt(shm_clk_curr_time);

    shmdt(shm_gen_num_arrived_processes);

    shmdt(shm_gen_processes_arrived);

    shmdt(shm_gen_alarm_time_out);

    shmdt(shm_schd_num_ready_processes);

    shmdt(shm_schd_running_process_remin_time);
    shmctl(shmid_schd_running_process_remin_time, IPC_RMID, 0);

    return;
}

