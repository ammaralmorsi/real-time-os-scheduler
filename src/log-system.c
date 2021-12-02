#include "../inc/process-input-information.h"
#include "../inc/log-system.h"

FILE* open_log_file(const char* file_name)
{
    return fopen(file_name, "w");
}

void close_log_file(FILE* fileptr)
{
    if(fileptr)
        fclose(fileptr);
    return;
}

void log_str(FILE* fptr, const char* str)
{
    if(fptr)
        fprintf(fptr, str);
    return;
}

void log_process_list(FILE* fptr, process_list* plist)
{
    int i;

    if(fptr && plist)
        for(i = 0; i < plist->size; i++)
            fprintf(fptr, "%d\t%d\t%d\t%d\n",
                    plist->processes[i].id,
                    plist->processes[i].arriv_time,
                    plist->processes[i].tot_run_time,
                    plist->processes[i].priority);
    return;
}
