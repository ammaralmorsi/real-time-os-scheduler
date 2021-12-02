#include <stdlib.h>
#include <stdio.h>

#include "../inc/process-input-information.h"

#define PRE_PROCESSED_FILE_NAME "processes.pre"

process_list* read_input_file(const char* file_name)
{
    process_list* plist;
    piinfo* processes;
    FILE* in_file, *pre_proc_file;
    int num_of_process, num_of_comments, num_of_lines, i;
    char c;

    in_file = fopen(file_name, "r");
    pre_proc_file = fopen(PRE_PROCESSED_FILE_NAME, "w");
    if(in_file)
    {
        if(pre_proc_file)
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

                putc(c ,pre_proc_file);
                
                if(c == '\n')
                    ++num_of_lines;

            }
            fclose(pre_proc_file);
        }
        fclose(in_file);
    }
    else
    {
        exit(EXIT_FAILURE);
    }

    num_of_process = num_of_lines - num_of_comments;
    processes = (piinfo*)malloc(num_of_process * sizeof(piinfo));

    pre_proc_file = fopen(PRE_PROCESSED_FILE_NAME, "r");
    if(pre_proc_file)
    {
        for(i = 0; i < num_of_process;i++)
            fscanf(pre_proc_file, "%d%d%d%d",
                    &(processes[i].id),
                    &(processes[i].arriv_time),
                    &(processes[i].tot_run_time),
                    &(processes[i].priority));

        fclose(pre_proc_file);
    }
    else
    {
        exit(EXIT_FAILURE);
    }

    plist = (process_list*)malloc(sizeof(process_list));

    plist->size = num_of_process;
    plist->processes = processes;

    return plist;
}


void free_input_resources(process_list** plist)
{
    if(*plist)
    {
        if((*plist)->processes)
            free((*plist)->processes);

        free(*plist);
        *plist = 0;
    }
    return;
}

