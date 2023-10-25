#ifndef SCHEDULERS_H
#define SCHEDULERS_H
#include "param.h"
#include "proc.h"
#include "spinlock.h"
struct ptable {
  struct spinlock lock;
  struct proc proc[NPROC];
};

// Declare the ptable instance as extern
extern struct ptable ptable;

int setticketshelper(struct proc*,int);
int random(int, int);
void roundrobin(void);
void lottery(void);

#endif // SCHEDULERS_H
