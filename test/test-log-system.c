#include "../inc/process-input-information.h"
#include "../inc/log-system.h"
#include "../inc/read-input-file.h"

int main(void)
{
    FILE* fptr;
    process_list* plist;

    plist = read_input_file("processes.txt");
    fptr = open_log_file("test.log");

    log_str(fptr, "THIS IS THE FIRST LINE IN THE LOG FILE\n");

    log_process_list(fptr, plist);

    close_log_file(fptr);
    free_input_resources(&plist);

    return 0;
}
