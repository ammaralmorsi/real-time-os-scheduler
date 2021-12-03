#ifndef PCB_QUEUE_H
#define PCB_QUEUE_H

#include "pcb.h"

typedef struct pcb_queue
{
    int size, read_index, write_index;
    PCB* pcbs;
} pcb_queue;

pcb_queue create_pcb_queue(const int);

void free_pcb_queue(pcb_queue*);

int is_full_pcb_queue(pcb_queue*);

int is_empty_pcb_queue(pcb_queue*);

void enqueue_pcb(pcb_queue*, PCB);

PCB dequeue_pcb(pcb_queue*);

#endif
