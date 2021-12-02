#include <stdio.h>
#include <assert.h>

#include "../inc/process-input-information.h"
#include "../inc/read-input-file.h"

int main(void)
{
    FILE* infile;
    process_list* plist;
    piinfo infolist[93];


    plist = read_input_file("processes.txt");

    infile = fopen("processes.pre", "r");
    if(infile)
    {
        for(int i = 0; i < 93; i++)
            fscanf(infile ,"%d %d %d %d",
                    &(infolist[i].id),
                    &(infolist[i].arriv_time),
                    &(infolist[i].tot_run_time),
                    &(infolist[i].priority));
        fclose(infile);
    }

    assert(plist);
    assert(plist->processes);
    assert(plist->size == 93);

    for(int i = 0; i < 93; i++)
        assert(plist->processes[i].id == infolist[i].id &&
                plist->processes[i].arriv_time == infolist[i].arriv_time &&
                plist->processes[i].tot_run_time == infolist[i].tot_run_time&&
                plist->processes[i].priority == infolist[i].priority);

    free_input_resources(&plist);
    assert(!plist);

    return 0;
}
