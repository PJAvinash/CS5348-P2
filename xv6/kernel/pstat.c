#include "pstat.h"
#include "defs.h"
#include "types.h"
#include "schedulers.h"

/*
** -- added by JXP220032
** helper function to fill input pointer with pstat info
** tries to aquire lock on on ptable
** locking outside this function and calling this might create deadlocks
*/
int getpinfohelper(struct pstat * pstatptr){
    if(pstatptr == NULL){
        return -1;
    }
    acquire(&ptable.lock);
    struct proc* p ;
    for (p = ptable.proc; p < &(ptable.proc[NPROC]); p++)
    {
        int index = p - ptable.proc;
        pstatptr->pid[index] = p->pid;
        if(p->state != UNUSED){
            pstatptr->inuse[index] = 1;
            pstatptr->tickets[index]= p->tickets;
            pstatptr->ticks[index]= p->ticks;  
        }else{
            pstatptr->inuse[index] = 0; 
            pstatptr->tickets[index] = 0;
            pstatptr->ticks[index] = 0; 
        }
    }
    release(&ptable.lock);
    return 0;
}