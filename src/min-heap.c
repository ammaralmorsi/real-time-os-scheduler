#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "../inc/pcb.h"
#include "../inc/min-heap.h"

static int get_parent_index_of_child_at(pcb_heap* heap, int i)
{
    if(!heap || i >= heap->length || i < 0)
        assert(0);

    if(i == 0)
        return -1;/*root*/

    return (i - 1)/2;
}

static int get_left_child_index_of_parent_at(pcb_heap* heap, int i)
{
    int index;

    if(!heap || i >= heap->length || i < 0)
        assert(0);

    index = 2*i + 1;
    if(index >= heap->length)
        return -1;

    return index;
}

static int get_right_child_index_of_parent_at(pcb_heap* heap, int i)
{
    int index;

    if(!heap || i >= heap->length || i < 0)
        assert(0);

    index = 2*i +2;
    if(index >= heap->length)
        return -1;
    
    return index;
}

static void sift_heap_up(pcb_heap* heap, int i)
{
    int parent_index;
    PCB temp;

    parent_index = get_parent_index_of_child_at(heap, i);

    if(parent_index == -1)
        return;
    
    if(heap->pcbs[parent_index].key > heap->pcbs[i].key)
    {
        temp = heap->pcbs[parent_index];
        heap->pcbs[parent_index] = heap->pcbs[i];
        heap->pcbs[i] = temp;
        sift_heap_up(heap, parent_index);
    }

    return;
}

static void sift_heap_down(pcb_heap* heap, int i)
{
    PCB temp;
    int left, right, min;

    left = get_left_child_index_of_parent_at(heap, i);
    right= get_right_child_index_of_parent_at(heap,i);

    if(left == -1 && right == -1) return;
    else if(left == -1) min = right;
    else if(right == -1) min = left;
    else if(heap->pcbs[left].key <= heap->pcbs[right].key) min = left;
    else min = right;

    if(heap->pcbs[min].key < heap->pcbs[i].key)
    {
        temp = heap->pcbs[min];
        heap->pcbs[min] = heap->pcbs[i];
        heap->pcbs[i] = temp;
        sift_heap_down(heap, min);
    }

    return;
}

pcb_heap create_pcb_heap(const int size)
{
    pcb_heap heap;

    heap.length = 0;
    heap.pcbs = (PCB*)malloc(size* sizeof(PCB));

    if(!heap.pcbs)
        exit(EXIT_FAILURE);

    return heap;
}

void free_pcb_heap(pcb_heap* heap)
{
    assert(heap);

    if(heap->pcbs)
        free(heap->pcbs);
    heap->pcbs = 0;
    return;
}

int is_empty_pcb_heap(pcb_heap* heap)
{
    assert(heap);

    return heap->length == 0;
}

PCB get_min_pcb_heap(pcb_heap* heap)
{
    if(is_empty_pcb_heap(heap))
        assert(0);/*not allowed operation*/

    return heap->pcbs[0];
}

void insert_pcb_heap(pcb_heap* heap, PCB pcb)
{
    if(!heap)
        assert(0);

    heap->pcbs[heap->length++] = pcb;
    sift_heap_up(heap, heap->length - 1);
}

PCB extract_min_pcb_heap(pcb_heap* heap)
{
    PCB temp;

    if(is_empty_pcb_heap(heap))
        assert(0);

    temp = get_min_pcb_heap(heap);
    heap->pcbs[0] = heap->pcbs[heap->length - 1];
    heap->length--;

    if(!is_empty_pcb_heap(heap))
        sift_heap_down(heap, 0);

    return temp;
}

