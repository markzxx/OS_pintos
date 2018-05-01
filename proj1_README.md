# OS_pintos Project 1
## Task 1: Effective Alarm
Task1要求实现一个基于闲等待的sleep函数
### Idea
要将忙等待变成闲等待，就需要将需要睡眠的线程暂时挂起不进行调度。等时间到了再重新加入就绪队了。因此我新增了一个等待队列来存放睡眠的线程，并且在每个ticks的时候去看一下有没有线程需要唤醒并将其唤醒加入就绪队列。
>
新增一条睡眠队列 **VS** 直接将线程标记为BLOCKED的好处如下：
+ 可以维护睡眠队列的有序性，让先唤醒的放在前面，这样每次只需看队列头就可以知道有没有线程需要被唤醒，否则每个tick都需要遍历所有线程。
### Data structure
1. 新增线程状态：THREAD_WATING，表示线程在sleep，与THREAD_BLOCKED等待锁的情况区别开来。
2. 新增一个list：wait_list，记录等待唤醒的线程，按预计唤醒时间排序。
### Method
1. 新增方法: thread_wait，将当前线程加入睡眠队列
```c
void thread_wait (int64_t ticks)
{
    struct thread *cur = thread_current ();
    enum intr_level old_level;
    ASSERT (!intr_context ());
    old_level = intr_disable ();
    cur->status = THREAD_WATING;
    cur->wake_tick = ticks;
    list_insert_ordered (&wait_list, &cur->wait_elem, thread_wake_comp, NULL);
    schedule ();
    intr_set_level (old_level);
}
```
2. 新增方法: thread_wake，查看睡眠队列头是否可唤醒，如果不可以break，如果可以则唤醒。如果被唤醒的线程优先级更高，则设置"intr_yield_on_return" flag
```c
void thread_wake (int64_t ticks){
    while (!list_empty(&wait_list)){
        struct thread *wake_thread = list_entry(list_front(&wait_list), struct thread, wait_elem);
        ASSERT(wake_thread->status == THREAD_WATING)
        if(wake_thread->wake_tick <= ticks)
        {
            list_insert_ordered(&ready_list, &wake_thread->elem, thread_priority_comp, NULL);
            list_pop_front(&wait_list);
            wake_thread->status = THREAD_READY;
            if(wake_thread->priority > thread_get_priority())
                intr_yield_on_return ();
        } else
            break;
    }
}
```
3. 修改方法： timer_sleep，如果tick小于等于0直接返回，否则调用thread_wait，传入参数为唤醒时间
```c
void timer_sleep (int64_t ticks)  //ticks是睡眠时间
{
    if(ticks<=0)
        return;
    thread_wait(timer_ticks ()+ticks); //加上timer_ticks（）获得的当前tick得到唤醒时间
}
```
4. 修改方法： timer_interrupt, 在每个timmer interrupt中调用thread_wake.
```c
static void timer_interrupt (struct intr_frame *args UNUSED)
{
    ticks++;
    thread_wake(ticks);  // 新增
    thread_tick ();
}
```
### Synchronization
+ 在将线程加入睡眠队列的时候，需要禁用中断，因为wait_list是一个共享的链表，需要避免race condition.
+ 唤醒线程的时候不需要考虑同步问题，因为是处在一个异常处理的过程中，不会被其他线程抢占。
## Task 2: Round-Robin scheduler
Task 2要求完成基于Round-Robin的调度策略，即每个线程运行一段时间后切换给下一个线程。
### Idea
原代码就有到了一个slice强制切换线程的实现，需要做的只是改为每个线程要有不同的slice长度，只需要新增一个变量保存起来即可。同时每次时间片用尽切换时，将自身优先级减3。
### Method
1. 修改方法：thread_tick，在thread中加入新变量：time_slice，在create_thread的时候赋值。每个线程连续执行slice个tick将强制切换，并自减优先级。
```c
if (++thread_ticks >= t->time_slice)
  {
      if(strcmp(t->name,"main")!=0)
      {
          t->priority = MAX(t->priority-3, 0);
          t->base_priority = MAX(t->base_priority-3, 0);
      }
      intr_yield_on_return ();
  }
```
## Task 3: Priority scheduler
Task 3要求按优先级调度，即优先级高的先调度，在此基础上还需要完成优先级捐赠，即高优先级任务由于需要一把由低优先级任务持有的锁时，将自身优先级临时捐赠给低优先级任务，让其尽快释放锁，以达到优先调度高优先级任务的目的。
### Idea
+ 首先，以下3种队列要从原来的无序队列改为按优先级排序的有序队列：
    + 就绪队列
    + 信号量等待队列
    + condition等待队列
+ 其次，每当如下4种情形发生的时候，要进行调度：
    + 新的高优先级线程被create
    + 睡眠状态的高优先级线程被wakeup
    + 等待锁或信号量的高优先级线程unblock
    + 当前线程reset自己的优先级
+ 最后，加入优先级捐赠和回收机制，添加donate_priority和donate_withdraw方法，并相应改写lock_acquire和lock_release方法。
### Data structure
1. 在struct Thread中加入
```c
int base_priority                 //记录捐赠前的优先级
struct list lock_list;            //记录当前线程获取了哪些锁
struct lock* lock;                //记录当前线程在等待哪个锁
struct semaphore* sema;           //记录当前线程在在等待哪个信号量
struct list* cond;                //记录当前线程在等待的condition
struct list_elem cond_waiter;     //记录当前线程所等待的condition队列中的链表指针
```
2. 在struct lock中加入
```c
int donate_priority;        //用于记录该锁被捐赠的最高优先级
struct list_elem elem;      //用于将这个锁加入thread的lock_list中
```
3. 在struct semaphore_elem中加入
```c
struct thread* thread;      //用于记录当前condition是哪个线程等待的，用于排序和更新优先级
```
### Method
1. 新增3个比较器，用于维护基于优先级的read_list, sema_waiters和lock_list
```c
//thread priority comperator
bool thread_priority_comp (const struct list_elem* a, const struct list_elem*b, void* aux UNUSED){
    return list_entry (a, struct thread, elem)->priority > 
    list_entry (b, struct thread, elem)->priority;
}
//lock priority comperator
bool lock_priority_comp (const struct list_elem* a,const struct list_elem*b, void* aux){
    return list_entry (a, struct lock, elem)->donate_priority > 
    list_entry (b, struct lock, elem)->donate_priority;
}
//condition priority conperator
bool cond_priority_comp (const struct list_elem* a,const struct list_elem*b, void* aux){
    return list_entry (a, struct semaphore_elem, elem)->thread->priority >
           list_entry (b, struct semaphore_elem, elem)->thread->priority;
}
```
2. 新增方法：thread_priority_yield，用于多种需要重新调度的情况，判断如果就绪队列中有比当前线程优先级更高的，就调用thread_yield进行调度。
```c
/*Yield, if any high priority thread in ready list */
void thread_priority_yield (void){
    if(!list_empty(&ready_list) &&
            list_entry(list_front(&ready_list),struct thread, elem)->priority > thread_get_priority())
            thread_yield();
}
```
3. 新增方法：list_reinsert，用于线程优先级被更新的时候，更新相应的队列。
```c
//remove e from l, and reinsert it. For update priority
void list_reinsert (struct list * l, struct list_elem* e, list_less_func * less, void *aux){
  list_remove(e);
  list_insert_ordered(l, e, less, aux);
}
```
4. 新增方法：thread_donate_priority, 用于向指定线程捐赠优先级，并且同时更新该线程在各队列中的排序。如果该线程被其他线程锁了，递归调用此方法。
```c
/*Donate priority to a thread */
void thread_donate_priority (struct thread *t, struct lock *l, int priority){
    l->donate_priority = priority;
    list_reinsert(&t->lock_list, &l->elem, lock_priority_comp, NULL);
    t->priority = priority;

    if(t->status == THREAD_READY)
        list_reinsert(&ready_list, &t->elem, thread_priority_comp, NULL);
    else if(t->cond != NULL)
        list_reinsert(t->cond, &t->cond_waiter, cond_priority_comp, NULL);
    else if(t->sema != NULL)
    {
        list_reinsert(&t->sema->waiters, &t->elem, thread_priority_comp, NULL);
        if(t->lock != NULL && t->lock->holder->priority < priority)
            thread_donate_priority(t->lock->holder, t->lock, priority);
    }
}
```
5. 新增方法：thread_donate_withdraw， 用于捐赠回收，回收后判断是否还持有其他锁，如果无，恢复回自己原来的优先级。如果有，取自己优先级和锁优先级中较大的那个。
```c
/*withdraw donated priority*/
void thread_donate_withdraw(struct lock *l){
    struct thread *t  = thread_current();
    l->donate_priority = -1;
    if(list_empty(&t->lock_list))
        t->priority = t->base_priority;
    else
        t->priority = MAX(t->base_priority, list_entry(list_front(&t->lock_list), struct lock, elem)->donate_priority);
}
```
6. 修改方法：thread_set_priority，如果锁队列为空，则直接将优先级设为新优先级，如果不为空，将base_priority设为新优先级，当前优先级设为锁队列第一个的优先级与新优先级的较大者。
```c
void thread_set_priority (int new_priority) 
{
    struct thread *t;
    enum intr_level old_level;

    old_level = intr_disable();
    t  = thread_current() ;
    t->base_priority = new_priority;
    if(list_empty(&t->lock_list))
        t->priority = new_priority;
    else
        t->priority = MAX(new_priority, list_entry(list_front(&t->lock_list), struct lock, elem)->donate_priority);

    thread_priority_yield();
    intr_set_level(old_level);
}
```
7. 修改方法：lock_acquire，如果锁被低于自己优先级的线程持有，则调用方法捐赠优先级。当自己拿到了一把锁，将该锁加入自己的锁队列中，以备后续被捐赠的时候需要用到。
```c
void lock_acquire (struct lock *lock)
{
    ASSERT (lock != NULL);
    ASSERT (!intr_context ());
    ASSERT (!lock_held_by_current_thread (lock));
    enum intr_level old_level;
    struct thread *t;

    old_level = intr_disable ();
    t = thread_current();
    if(lock->holder != NULL) {
        t->lock = lock;
        if (lock->holder->priority < t->priority)
            thread_donate_priority(lock->holder, lock, t->priority);
    }
    sema_down (&lock->semaphore);
    lock->holder = t;
    list_insert_ordered(&t->lock_list, &lock->elem, lock_priority_comp, NULL);
    intr_set_level (old_level);
}
```
8. 修改方法: lock_release，释放锁时，将锁从自己的锁列表移除，如果被捐赠过，则调用方法回收捐赠。
```c
void lock_release (struct lock *lock) 
{
    ASSERT (lock != NULL);
    ASSERT (lock_held_by_current_thread (lock));
    enum intr_level old_level;
    old_level = intr_disable ();
    list_remove(&lock->elem);
    if(lock->donate_priority != -1)
      thread_donate_withdraw(lock);
    lock->holder = NULL;
    sema_up (&lock->semaphore);
    intr_set_level (old_level);
}
```
9. 修改方法sema_down, cond_wait将插入规则改成有序插入。
10. 修改方法sema_up, thread_create, thread_set_priority，在方法最后调用thread_priority_yield进行调度。
### Synchronization
写不动了。略

## Hack Test
做的过程中想到了几个可能被忽略的细节，可以作为hack test
+ 在被捐赠的情况下，设置自己的优先级，设置值比捐赠高：
    + Task A优先级为30，持有锁。
    + Task B优先级为40，需要锁，捐赠A。此时A的优先级为40。
    + A在运行过程中，调用set_priority方法设置自己的优先级为50。此时新优先级比捐赠优先级高，应直接生效，此时A的优先级为50。 
+ 在被捐赠的情况下设置自己的优先级，设置值介于2个捐赠之间：
    + Task A优先级为30，持有b、c两个锁。
    + Task B优先级为40，需要锁b，捐赠A，此时A的优先级为40。
    + Task C优先级为50，需要锁c，捐赠A，此时A的优先级为50。
    + A在运行过程中，调用set_priority方法设置自己的优先级为45，由于45小于50，暂时不生效。此时A的优先级仍然为50。
    + A释放锁c，捐赠被回收，区别于测试点donate-multiple，此时A的优先级应降为自己设置的45而非B捐赠的40。
