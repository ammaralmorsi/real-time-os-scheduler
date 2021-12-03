#ifndef PCB_H
#define PCB_H

#include "process-input-information.h"

typedef enum process_state
{
    NONE_PS = 0,
    STARTED,
    RESUMED,
    STOPPED,
    FINISHED
} process_state;

typedef struct process_control_block
{
    process_state curr_state;
    piinfo given_info; /*given from the generator*/
    int last_time_leave_cpu,
        remain_time,
        wait_time,
        turn_around_time;
    float weighted_turn_around;
} PCB;

#endif

