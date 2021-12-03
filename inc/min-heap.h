#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include "pcb.h"

typedef struct pcb_min_heap
{
    int length;
    PCB* pcbs;

} pcb_heap;

pcb_heap create_pcb_heap(const int);

void free_pcb_heap(pcb_heap*);

int is_empty_pcb_heap(pcb_heap*);

PCB get_min_pcb_heap(pcb_heap*);

PCB extract_min_pcb_heap(pcb_heap*);

void insert_pcb_heap(pcb_heap*, PCB);

#endif
