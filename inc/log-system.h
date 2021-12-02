#ifndef LOG_SYSTEM_H
#define LOG_SYSTEM_H

#include <stdio.h>

#include "process-input-information.h"

FILE* open_log_file(const char*);

void close_log_file(FILE*);

void log_str(FILE*, const char*);

void log_process_list(FILE*, process_list*);

#endif
