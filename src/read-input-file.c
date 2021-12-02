#include <stdlib.h>
#include <stdio.h>

#include "../inc/process-input-information.h"

piinfo* read_input_file(const char* file_name)
{
    piinfo* processes;
    int num_of_process, num_of_comments, num_of_lines;
    FILE* in_file, *pre_processed_file;
    char c;
    int i;

    /*
     * first open the file to count number of lines
     * and number of comments
     */

    in_file = fopen(file_name, "r");
    pre_processed_file = fopen("pre.txt", "w");
    if(in_file)
    {
        if(pre_processed_file)
        {
            num_of_lines = num_of_comments = 0;
            while((c=getc(in_file)) != EOF)
            {

                if(c == '#')
                {
                    ++num_of_comments;
                    /*ignore until the end of the line*/
                    while((c = getc(in_file)) != EOF &&
                            c != '\n')
                        ;
                    ++num_of_lines;
                    continue;
                }

                if(c == '\n')
                    ++num_of_lines;

                putc(c ,pre_processed_file);
            }
            fclose(pre_processed_file);
        }
        fclose(in_file);
    }
    else
    {
        exit(EXIT_FAILURE);
    }
    printf("number of lines:%d \n", num_of_lines);
    printf("number of comments:%d \n", num_of_comments);

    num_of_process = num_of_lines - num_of_comments;
    printf("number of processes: %d\n", num_of_process);

    processes = (piinfo*)malloc(num_of_process * sizeof(piinfo));

    pre_processed_file = fopen("pre.txt", "r");
    if(pre_processed_file)
    {
        for(i = 0; i < num_of_process;i++)
            fscanf(pre_processed_file, "%d%d%d%d",
                    &(processes[i].id),
                    &(processes[i].arriv_time),
                    &(processes[i].tot_run_time),
                    &(processes[i].priority));

        fclose(pre_processed_file);
    }
    else
    {
        exit(EXIT_FAILURE);
    }

    for(i = 0; i < num_of_process; i++)
        printf("%d %d %d %d\n",
                processes[i].id,
                processes[i].arriv_time,
                processes[i].tot_run_time,
                processes[i].priority);

    return 0;  
}

