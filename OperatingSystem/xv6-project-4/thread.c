#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"

#define NTHREAD 8

int
thread_create(void (*function)(void*), void *arg, void *stack){
  int i, tid;
  struct proc *np;
  struct proc *p = proc;

  if(proc->numofthread > NTHREAD - 1){
    return -1;
  }
  
  if((np = allocproc()) == 0){
    return -1;
  }
  np->numofthread = ++proc->numofthread;
  np->pgdir = proc->pgdir;
  np->sz = proc->sz;

  while(p->isthread != 0){
    p=p->parent;
  }
  np->parent = p;
  
  np->nice = proc->nice;
  *np->tf = *proc->tf;
  np->pid = proc->pid;
  np->isthread = 1;

  for(i = 0; i < NOFILE; i++)
    if(proc->ofile[i])
      np->ofile[i] = filedup(proc->ofile[i]);
  np->cwd = idup(proc->cwd);
  safestrcpy(np->name, proc->name, sizeof(proc->name));


  np->tf->eax = 0;
  np->tid = nexttid++;
  tid = np->tid;
  
  // Set Function, Stack (OK)
  np->thread_stack = (int)stack;
  np->tf->eip = (int)function;
  acquire(&ptable.lock);

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->pid == np->pid && p != np && p->isthread){
      p->numofthread++;
    }
  }
  release(&ptable.lock);
  // Set Argument(not work?)
  *(int *)((int)stack + 4096 - sizeof(int)) = (int)arg; // move esp to the top of the new stack
  *(int *)((int)stack + 4096 - 2 * sizeof(int)) = proc->tf->eip; // push the return address
  np->tf->esp = (int)stack + 4096 - 2 * sizeof(int);

  acquire(&ptable.lock);

  np->state = RUNNABLE;

  release(&ptable.lock);
  return tid;
}

void
thread_exit(void *retval){
  struct proc *p;

  if(proc == initproc)
    panic("init exiting");

  acquire(&ptable.lock);

  // Parent might be sleeping in wait().
  wakeup1(proc->parent);

  // Pass abandoned children to init.
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->parent == proc){
      p->parent = proc->parent;
    }
  }
  proc->retval = retval;

  // Jump into the scheduler, never to return.
  proc->state = ZOMBIE;
  sched();
  panic("zombie exit");
}

int
thread_join(int tid, void **retval){
  if(tid == -1)
    return -1;
  struct proc *p;
  acquire(&ptable.lock);
  for(;;){
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->tid == tid && p->state == ZOMBIE){
        if(retval != 0)
          *retval = p->retval;
        kfree(p->kstack);
        p->kstack = 0;
        p->pid=0;
        p->tid = 0;
        p->parent = 0;
        p->name[0] = 0;
        p->killed = 0;
        p->state = UNUSED;
        proc->numofthread--;
        release(&ptable.lock);
        return 0;
      }
    }
    // No point waiting if we don't have any children.
    if(proc->killed){
      release(&ptable.lock);
      return -1;
    }

    // Wait for children to exit.  (See wakeup1 call in proc_exit.)
    sleep(proc, &ptable.lock);  //DOC: wait-sleep
  }
}

int
gettid(){
  return proc->tid;
}