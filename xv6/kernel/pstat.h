#ifndef _PSTAT_H_
#define _PSTAT_H_
#include "param.h"
/*
** added  by JXP220032
** pstat struct definition
** caller to getpinfo should initialize pinter to this struct;
*/
struct pstat { 
    int inuse[NPROC]; // whether this slot of the process table is in use (1 or 0) 
    int tickets[NPROC]; // the number of tickets this process has 
    int pid[NPROC]; // the PID of each process 
    int ticks[NPROC]; // the number of ticks each process has accumulated 
};
int getpinfohelper(struct pstat *);

#endif //_PSTAT_H_