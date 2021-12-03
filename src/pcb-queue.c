#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../inc/pcb-queue.h"

pcb_queue create_pcb_queue(const int size)
{
    pcb_queue queue;

    queue.pcbs = (PCB*)malloc(size * sizeof(PCB));
    
    if(!queue.pcbs)
        exit(EXIT_FAILURE);

    queue.size = size;
    queue.read_index = -1;
    queue.write_index = 0;
    
    return queue;
}

void free_pcb_queue(pcb_queue* queue)
{
    if(queue->pcbs)
        free(queue->pcbs);
    queue->pcbs = 0;
    return;
}

int is_full_pcb_queue(pcb_queue* queue)
{
    return queue->write_index == -1;
}

int is_empty_pcb_queue(pcb_queue* queue)
{
    return queue->read_index == -1;
}

void enqueue_pcb(pcb_queue* queue, PCB pcb)
{
    if(!queue->pcbs || is_full_pcb_queue(queue))
        assert(0); /*not allowed operation*/

    if(is_empty_pcb_queue(queue))
        queue->read_index = queue->write_index;
    queue->pcbs[queue->write_index] = pcb;
    queue->write_index = (queue->write_index + 1) % queue->size;
    return;
}

PCB dequeue_pcb(pcb_queue* queue)
{
    PCB pcb;

    if(!queue->pcbs || is_empty_pcb_queue(queue))
        assert(0); /*not allowed operation*/

    if(is_full_pcb_queue(queue))
        queue->write_index = queue->read_index;
    pcb = queue->pcbs[queue->read_index];
    queue->read_index = (queue->read_index+1)%queue->size;

    return pcb;
}
