#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "../inc/process-input-information.h"
#include "../inc/log-system.h"
#include "../inc/read-input-file.h"
#include "../inc/pcb.h"
#include "../inc/pcb-queue.h"
#include "../inc/min-heap.h"

int main(void)
{
    FILE* fptr, *hfptr;
    process_list* plist;
    PCB* pcb_test, holder;
    pcb_queue queue;
    pcb_heap heap;

    plist = read_input_file("processes.txt");
    fptr = open_log_file("test.log");
    hfptr = open_log_file("heap.log");
    pcb_test = (PCB*)malloc(plist->size * sizeof(PCB));
    queue = create_pcb_queue(plist->size);
    heap = create_pcb_heap(plist->size);


    log_str(fptr, "THIS IS THE FIRST LINE IN THE LOG FILE\n");

    log_process_list(fptr, plist);

    assert(is_empty_pcb_heap(&heap));

    for(int i = 0; i < plist->size; i++)
    {
        
        pcb_test[i].curr_state = i%5;
        pcb_test[i].given_info.id = plist->processes[i].id;
        pcb_test[i].given_info.arriv_time = plist->processes[i].arriv_time;
        pcb_test[i].given_info.tot_run_time = plist->processes[i].tot_run_time;
        pcb_test[i].given_info.priority = plist->processes[i].priority;
        pcb_test[i].last_time_leave_cpu = i;
        pcb_test[i].remain_time = i;
        pcb_test[i].wait_time = i;
        pcb_test[i].turn_around_time = i;
        pcb_test[i].weighted_turn_around = 1.333f * i;
        pcb_test[i].key = plist->processes[i].arriv_time; 

        insert_pcb_heap(&heap, pcb_test[i]);

        enqueue_pcb(&queue, pcb_test[i]);
        holder = dequeue_pcb(&queue);
        log_pcb(fptr, i, &holder);
    }

    for(int i = 0; i < plist->size; i++)
    {
        PCB temp;

        temp = extract_min_pcb_heap(&heap);
        log_pcb(hfptr, i, &temp);
    }


    free_pcb_heap(&heap);
    free_pcb_queue(&queue);
    close_log_file(fptr);
    close_log_file(hfptr);
    free_input_resources(&plist);
    free(pcb_test);

    return 0;
}
