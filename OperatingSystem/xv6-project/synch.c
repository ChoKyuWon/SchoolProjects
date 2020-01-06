#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#include "synch.h"

#define DEBUG_M 0
#define DEBUG_C 0
#define DEBUG_C_SIGNAL 0

int
mutex_init(struct mutex_t *mutex)
{
  if(mutex->valid == 1){
    return -2;
  }
  else{
    mutex->valid = 1;
  }
  int i;
  initlock(&mutex->lock, "mutex");
  mutex->current = 0;
  mutex->qsize = 0;
  mutex->rear = 0;
  mutex -> head = 0;
  for(i=0;i<NTHREAD-1;i++){
    mutex->queue[i] = 0;
  }
  return 0;
}

int
mutex_lock(struct mutex_t *mutex)
{
  struct proc *cur = myproc();
  if(mutex->current == cur)
    return -3;
  if(mutex->valid != 1)
    return -2;
  // mutex implement
  if(DEBUG_M)
    cprintf("   DEBUG) %s %d try to get mutex...\n", cur->name, cur->tid);
  acquire(&mutex->lock);
  if(DEBUG_M)
    cprintf("   DEBUG) %s %d get mutex's spinlock!\n", cur->name, cur->tid);
  if(mutex->current == 0){
    mutex->current = cur;
    if(DEBUG_M)
      cprintf("   DEBUG) now %s %d get mutex and release spinlock!!\n", cur->name, cur->tid);
    release(&mutex->lock);
    return 0;
  }
  while(mutex->current != cur){
    if(DEBUG_M)
      cprintf("   DEBUG) IN current, %s %d is running...\n", mutex->current->name, mutex->current->tid);
    
    if(mutex->rear == mutex->head && mutex->qsize != 0){
      if(DEBUG_M)
        cprintf("   DEBUG) Queue is Full! %s %d is release spinlock and return -1.\n", mutex->current->name, mutex->current->tid);
      release(&mutex->lock);
      return -1;
    }
    mutex->queue[mutex->rear] = cur;
    mutex->rear++;
    if(mutex->rear >= NTHREAD -1)
      mutex->rear -= NTHREAD -1;
    mutex->qsize++;
    if(DEBUG_M){
      cprintf("   DEBUG) In wating queue,");
      int i;
      for(i = 0; i<7 ; i++){
        if(mutex->queue[i] != 0)
          cprintf("%d ", mutex->queue[i]->tid);
        else
          cprintf("0 ");
      }
      cprintf(", HEAD is %d, REAR is %d, qsize is %d.\n", mutex->head, mutex->rear, mutex->qsize);
    }
    sleep(mutex, &mutex->lock);
  }

  mutex->current = cur;
  release(&mutex->lock);
  if(DEBUG_M)
    cprintf("   DEBUG) %s %d got mutex! and release spinlock\n", cur->name, cur->tid);
  return 0;
}

int
mutex_unlock(struct mutex_t *mutex)
{
  struct proc *cur = myproc();
  if(DEBUG_M)
    cprintf("   DEBUG) now %s %d try to unlock mutex...\n", cur->name, cur->tid);

  if(mutex->valid != 1)
    return -2;
  if(mutex->current != cur){
    if(DEBUG_M)
      cprintf("   DEBUG) %s %d dosen't have mutex! return -3. current %s %d has mutex.\n", cur->name, cur->tid, mutex->current->name, mutex->current->tid);
    return -3;
  }

  // mutex implement
  acquire(&mutex->lock);
  if(DEBUG_M){
    cprintf("   DEBUG) now %s %d get mutex's spinlock!!\n", cur->name, cur->tid);
    cprintf("   DEBUG) In wating queue,");
    int i;
    for(i = 0; i<7 ; i++){
      if(mutex->queue[i] != 0)
        cprintf("%d ", mutex->queue[i]->tid);
      else
        cprintf("0 ");
    }
    cprintf(", HEAD is %d, REAR is %d, qsize is %d.\n", mutex->head, mutex->rear, mutex->qsize);
  }
  // if(mutex->next != 0){
  //   mutex->current = mutex->next;
  //   mutex->current->state = RUNNABLE;
  //   mutex->next = 0;
  // }
  if(mutex->qsize == 0){
    mutex->current = 0;
    if(DEBUG_M)
      cprintf("   DEBUG) Nobody in wating queue! now %s %d release mutex's spinlock and unlock mutex!!\n", cur->name, cur->tid);
    release(&mutex->lock);
    return 0;
  }
  mutex->current = mutex->queue[mutex->head];
  mutex->head++;
  if(mutex->head >= NTHREAD -1){
    mutex->head -= NTHREAD -1;
  }
  mutex->current->state = RUNNABLE;
  mutex->qsize--;
  if(DEBUG_M)
    cprintf("   DEBUG) %s %d is wakeup!\n", mutex->current->name, mutex->current->tid);
  release(&mutex->lock);
  return 0;
}

int
cond_init(struct cond_t *cond){
  if(cond->active == 1){
    return -2;
  }

  int i;
  cond->active = 1;
  initlock(&cond->lock, "cond");
  for(i = 0;i < NTHREAD -1; i++){
    cond->s_queue[i] = 0;
  }
  cond->s_qsize = 0;
  cond->s_head = 0;
  cond->s_rear = 0;
  return 0;
}

int
cond_wait(struct cond_t *cond, struct mutex_t *mutex){
  if(cond->active != 1){
    return -2;
  }
  struct proc *cur = myproc();
  if(DEBUG_C)
    cprintf("   DEBUG_C) %s %d try to wait cond...\n", cur->name, cur->tid);
  acquire(&cond->lock);
  if(DEBUG_C)
    cprintf("   DEBUG_C) %s %d get cond_lock.\n", cur->name, cur->tid);
  
  if(cond->s_qsize != 0 && cond->s_head == cond->s_rear){
    if(DEBUG_C)
      cprintf("   DEBUG_C) Q is full!\n");
    release(&cond->lock);
    return -1;
  }
  cond->s_queue[cond->s_rear] = cur;
  cond->s_rear++;
  if(cond->s_rear >= NTHREAD - 1)
    cond->s_rear -= NTHREAD - 1;
  cond->s_qsize++;
  if(DEBUG_C){
    cprintf("   DEBUG_C) In condition wating queue,");
    int i;
    for(i = 0; i<7 ; i++){
      if(cond->s_queue[i] != 0)
        cprintf("%d ", cond->s_queue[i]->tid);
      else
        cprintf("0 ");
    }
    cprintf(", HEAD is %d, REAR is %d, qsize is %d.\n", cond->s_head, cond->s_rear, cond->s_qsize);
  }
  int state = mutex_unlock(mutex);
  if(state != 0){
    if(DEBUG_C)
      cprintf("   DEBUG_C) Fail to unlock mutex, %d state\n", state);
    release(&cond->lock);
    return -3;
  }
  if(DEBUG_C)
    cprintf("   DEBUG_C) %s %d goto sleep.\n", cur->name, cur->tid);
  //mutex->next = cur;
  sleep(cond, &cond->lock);
  release(&cond->lock);
  mutex_lock(mutex);
  return 0;
}

int
cond_signal(struct cond_t *cond){
  if(cond->active != 1){
    return -2;
  }
  struct proc *cur = myproc();
  struct proc *p;
  if(DEBUG_C_SIGNAL)
    cprintf("   DEBUG_C) %s %d send signal.\n",cur->name, cur->tid);
  acquire(&cond->lock);
  if(DEBUG_C_SIGNAL){
    cprintf("   DEBUG_C) %s %d get cond_lock.\n",cur->name, cur->tid);
    cprintf("   DEBUG_C) In condition wating queue,");
    int i;
    for(i = 0; i<7 ; i++){
      if(cond->s_queue[i] != 0)
        cprintf("%d ", cond->s_queue[i]->tid);
      else
        cprintf("0 ");
    }
    cprintf(", HEAD is %d, REAR is %d, qsize is %d.\n", cond->s_head, cond->s_rear, cond->s_qsize);
  }
  if(cond->s_qsize != 0){
    p = cond->s_queue[cond->s_head];
    cond->s_head++;
    if(cond->s_head >= NTHREAD - 1)
      cond->s_head -= NTHREAD - 1;
    cond->s_qsize--;
    p->state = RUNNABLE;
  }
  if(DEBUG_C_SIGNAL)
    cprintf("   DEBUG_C) %s %d release conditional variable lock\n",cur->name, cur->tid);
  release(&cond->lock);
  return 0;
}



/*int
mutex_lock(struct mutex_t *mutex)
{
  struct proc *cur = myproc();
  if(mutex->current == cur)
    return -3;
  if(mutex->valid != 1)
    return -2;
  // mutex implement
  if(DEBUG_M)
    cprintf("   DEBUG) %s %d try to get mutex...\n", cur->name, cur->tid);
  acquire(&mutex->lock);
  if(DEBUG_M)
    cprintf("   DEBUG) %s %d get mutex's spinlock!\n", cur->name, cur->tid);
  if(mutex->current == 0){
    mutex->current = cur;
    if(DEBUG_M)
      cprintf("   DEBUG) now %s %d get mutex and release spinlock!!\n", cur->name, cur->tid);
    release(&mutex->lock);
    return 0;
  }
  while(mutex->current != cur){
    if(DEBUG_M)
      cprintf("   DEBUG) IN current, %s %d is running...\n", mutex->current->name, mutex->current->tid);
    
    if(mutex->rear == mutex->head && mutex->qsize != 0){
      if(DEBUG_M)
        cprintf("   DEBUG) Queue is Full! %s %d is release spinlock and return -1.\n", mutex->current->name, mutex->current->tid);
      release(&mutex->lock);
      return -1;
    }
    mutex->queue[mutex->rear] = cur;
    mutex->rear++;
    if(mutex->rear >= NTHREAD -1)
      mutex->rear -= NTHREAD -1;
    mutex->qsize++;
    if(DEBUG_M){
      cprintf("   DEBUG) In wating queue,");
      int i;
      for(i = 0; i<7 ; i++){
        if(mutex->queue[i] != 0)
          cprintf("%d ", mutex->queue[i]->tid);
        else
          cprintf("0 ");
      }
      cprintf(", HEAD is %d, REAR is %d, qsize is %d.\n", mutex->head, mutex->rear, mutex->qsize);
    }
    sleep(mutex, &mutex->lock);
  }

  mutex->current = cur;
  release(&mutex->lock);
  if(DEBUG_M)
    cprintf("   DEBUG) %s %d got mutex! and release spinlock\n", cur->name, cur->tid);
  return 0;
}

int
mutex_unlock(struct mutex_t *mutex)
{
  struct proc *cur = myproc();
  if(mutex->current != cur){
    if(DEBUG_M)
      cprintf("   DEBUG) %s %d dosen't have mutex! return -3. current %s %d has mutex.\n", cur->name, cur->tid, mutex->current->name, mutex->current->tid);
    return -3;
  }
  if(mutex->valid != 1)
    return -2;

  // mutex implement
  if(DEBUG_M)
    cprintf("   DEBUG) now %s %d try to unlock mutex...\n", cur->name, cur->tid);
  acquire(&mutex->lock);
  if(DEBUG_M){
    cprintf("   DEBUG) now %s %d get mutex's spinlock!!\n", cur->name, cur->tid);
    cprintf("   DEBUG) In wating queue,");
    int i;
    for(i = 0; i<7 ; i++){
      if(mutex->queue[i] != 0)
        cprintf("%d ", mutex->queue[i]->tid);
      else
        cprintf("0 ");
    }
    cprintf(", HEAD is %d, REAR is %d, qsize is %d.\n", mutex->head, mutex->rear, mutex->qsize);
  }
  if(mutex->qsize == 0){
    mutex->current = 0;
    if(DEBUG_M)
      cprintf("   DEBUG) Nobody in wating queue! now %s %d release mutex's spinlock and unlock mutex!!\n", cur->name, cur->tid);
    release(&mutex->lock);
    return 0;
  }
  mutex->current = mutex->queue[mutex->head];
  mutex->head++;
  if(mutex->head >= NTHREAD -1){
    mutex->head -= NTHREAD -1;
  }
  //mutex->current->state = RUNNABLE;
  mutex->qsize--;
  if(DEBUG_M)
    cprintf("   DEBUG) %s %d is wakeup!\n", mutex->current->name, mutex->current->tid);
  release(&mutex->lock);
  return 0;
}*/