#include <stdio.h>
#include <assert.h>

#include "../inc/algorithm-type.h"
#include "../inc/get-algorithm.h"

int main(void)
{
    algo_type alg;
    char c;

    c = 'y';
    while(c != 'n')
    {
        alg = get_algorithm_type();
        switch(alg)
        {
            case HPF:
                printf("HPF\n");
                break;
            case SRTN:
                printf("SRTN\n");
                break;
            case RR:
                printf("RR\n");
                break;
            default:
                printf("none\n");
                break;
        }
        printf("again?(y, n)\n");
        scanf("%c", &c);
        while(c != '\n' && getchar() != '\n')
            ;

    }

    return 0;
}
