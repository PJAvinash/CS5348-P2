#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
#include "schedulers.h"
// added by JXP220032
struct ptable {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

/*
** setticketshelper is used for setting tickets to a process 
** assumes the caller holds the lock on ptable
** return -1 on failure and 0 on success ( unlike 0 and 1 for failure and success respectively)
*/
int setticketshelper(struct proc* procptr, int numtickets){
    if(numtickets < 1){
        return -1;
    }else{
        procptr->tickets = numtickets;
        return 0;
    }
}

#define LCG_MULTIPLIER 1664525
#define LCG_INCREMENT 1013904223
#define LCG_MODULUS ((unsigned int)1 << 31)

/*
** Generate a psuedo random number between 'from' and 'to' (inclusive)
*/
int random(int from, int to) {
    if (from > to) {
        // Swap 'from' and 'to' if 'from' is greater than 'to'
        int temp = from;
        from = to;
        to = temp;
    }
    static unsigned int state = 0;
    state = (LCG_MULTIPLIER * state + LCG_INCREMENT) % LCG_MODULUS;
    return from + (int)(state % (to - from + 1));
}

/*
** roundrobin() is the default round-robin scheduler in xv6. the following code is extracted from original xv6 implementation
*/
void roundrobin(){
    struct proc *p;
    sti();
    acquire(&ptable.lock);
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->state != RUNNABLE)
        continue;

      // Switch to chosen process.  It is the process's job
      // to release ptable.lock and then reacquire it
      // before jumping back to us.
      proc = p;
      switchuvm(p);
      p->state = RUNNING;
      swtch(&cpu->scheduler, proc->context);
      switchkvm();

      // Process is done running for now.
      // It should have changed its p->state before coming back.
      proc = 0;
    }
    release(&ptable.lock);
}

/*
** lottery() is the lottery scheduler , uses psuedo-random numbers generated to pick the winner
*/
void lottery(){
    struct proc *p;
    sti();
    acquire(&ptable.lock);
    int totaltickets = 0;
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
        if(p->state == RUNNABLE){
            totaltickets += p->tickets;
        }
    }
    int new_pick = random(1,totaltickets);
    for(p = ptable.proc; p < &ptable.proc[NPROC] && new_pick > 0; p++){
        if(p->state == RUNNABLE){
            new_pick -= p->tickets;
        }
    }
    p--;
    proc = p;
    switchuvm(p);
    p->state = RUNNING;
    p->ticks = p->ticks + 1; // update num ticks for stats
    swtch(&cpu->scheduler, proc->context);
    switchkvm();
    proc = 0; 
    release(&ptable.lock);

}



