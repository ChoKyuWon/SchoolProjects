#define NTHREAD 8

struct mutex_t {
  int valid;
  int locked;
  struct spinlock lock;
  struct proc *current;
  struct proc *queue[NTHREAD - 1];
  int qsize;
  int head;
  int rear;
};

struct cond_t{
    int active;
    struct spinlock lock;
    struct proc *s_queue[NTHREAD -1];
    int s_qsize;
    int s_head;
    int s_rear;
};