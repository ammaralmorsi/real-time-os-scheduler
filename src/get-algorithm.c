#include <stdio.h>

#include "../inc/algorithm-type.h"
#include "../inc/get-algorithm.h"

algo_type get_algorithm_type(void)
{
    algo_type alg;
    char c;

    printf("Input Algorithm type..\n"
            "Highest Priority First(h)\n"
            "Shortest Remaining Time Next(s)\n"
            "Round Robin(r)\n");
    scanf("%c", &c);
    while(c != '\n' &&  getchar() != '\n')
        ;/*ignore until the end of the line*/

    switch(c)
    {
        case 'h':
        case 'H':
            alg = HPF;
            break;
        case 's':
        case 'S':
            alg = SRTN;
            break;
        case 'r':
        case 'R':
            alg = RR;
            break;
        default:
            alg = NONE;
            break;
    }

    return alg;
}

