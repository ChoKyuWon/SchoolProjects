#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"


int
thread_create(void (*function)(void*), void *arg, void *stack){
  int i, tid;
  struct proc *np;
  struct proc *curproc = myproc();
  struct proc *p = curproc;

  if((np = allocproc()) == 0){
    return -1;
  }
  if(curproc->numofthread > NTHREAD - 1){
    return -1;
  }
  np->numofthread = ++curproc->numofthread;
  np->pgdir = curproc->pgdir;
  np->sz = curproc->sz;

  while(p->isthread != 0){
    p=p->parent;
  }
  np->parent = p;
  
  np->nice = curproc->nice;
  *np->tf = *curproc->tf;
  np->pid = curproc->pid;
  np->isthread = 1;

  for(i = 0; i < NOFILE; i++)
    if(curproc->ofile[i])
      np->ofile[i] = filedup(curproc->ofile[i]);
  np->cwd = idup(curproc->cwd);
  safestrcpy(np->name, curproc->name, sizeof(curproc->name));


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
  *(int *)((int)stack + 4096 - 2 * sizeof(int)) = curproc->tf->eip; // push the return address
  np->tf->esp = (int)stack + 4096 - 2 * sizeof(int);

  acquire(&ptable.lock);

  np->state = RUNNABLE;

  release(&ptable.lock);
  return tid;
}

void
thread_exit(void *retval){
  struct proc *curproc = myproc();
  struct proc *p;

  if(curproc == initproc)
    panic("init exiting");

  acquire(&ptable.lock);

  // Parent might be sleeping in wait().
  wakeup1(curproc->parent);

  // Pass abandoned children to init.
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->parent == curproc){
      p->parent = curproc->parent;
    }
  }
  curproc->retval = retval;

  // Jump into the scheduler, never to return.
  curproc->state = ZOMBIE;
  sched();
  panic("zombie exit");
}

int
thread_join(int tid, void **retval){
  if(tid == -1)
    return -1;
  struct proc *p;
  struct proc *curproc = myproc();
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
        curproc->numofthread--;
        release(&ptable.lock);
        return 0;
      }
    }
    // No point waiting if we don't have any children.
    if(curproc->killed){
      release(&ptable.lock);
      return -1;
    }

    // Wait for children to exit.  (See wakeup1 call in proc_exit.)
    sleep(curproc, &ptable.lock);  //DOC: wait-sleep
  }
}

int
gettid(){
  struct proc * curproc = myproc();
  return curproc->tid;
}