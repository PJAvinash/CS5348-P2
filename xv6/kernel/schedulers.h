#ifndef SCHEDULERS_H
#define SCHEDULERS_H
#include "param.h"
#include "proc.h"
#include "spinlock.h"
struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

// Declare the ptable instance as extern
//extern struct ptable ptable;

#endif // SCHEDULERS_H
