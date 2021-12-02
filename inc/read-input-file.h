#ifndef READ_INPUT_FILE_H
#define READ_INPUT_FILE_H

#include "process-input-information.h"

/*
 * Input: name of input file
 * Output: dynamically allocated array of process_input_information structure
 *          caller should be responsible for free memory
 */
piinfo* read_input_file(const char*);

#endif

