#ifndef SIM_SHM_H
#define SIM_SHM_H

#include <sys/ipc.h>
#include <sys/shm.h>

#define FTOK_FILE               "./Makefile"

#define SHM_CREATOR_FLAG        IPC_CREAT | IPC_EXCL | 0666
#define SHM_NOT_CREATOR_FLAG    0666

#endif
