#include <pthread.h>
#include <stdio.h>
#include <stdatomic.h>
#include <math.h>

typedef volatile struct {
  volatile atomic_int val;
  volatile atomic_flag mut;
} mysem_t; // Semaphore

typedef atomic_flag mymut_t; // Mutex

#define acquire(m) while (atomic_flag_test_and_set(m)) // Mutex Lock
#define release(m) atomic_flag_clear(m) // Mutex Release/Unlock

int wait(mysem_t * s) {
  acquire(&s->mut);                 
  while (atomic_load(&s->val) <= 0);
  atomic_fetch_sub(&s->val, 1);
  release(&s->mut);
  return 0;
} // wait

int signal(mysem_t * s) {
  atomic_fetch_add(&s->val, 1);
  return 0;
} // signal

int init(mysem_t * s, int value){
  atomic_init(&s->val, value);
  return 0;
} // init

/* 
    writer_mutex: semaphore for locking resource from writers;
    reader_mutex: semaphore for locking resource for indi writer;
    queue_mutex: semaphore for queue maintaining
*/

mysem_t queue_mutex;

/*
    A mutex (named for "mutual exclusion") is a binary semaphore with an ownership restriction:
    it can be unlocked (the signal operation) only by whoever locked it (the wait operation). 
    Thus a mutex offers a somewhat stronger protection than an ordinary semaphore.
    We declare a mutex as:
    mymut_t mutex;
*/

volatile mymut_t reader_mutex = ATOMIC_FLAG_INIT, writer_mutex = ATOMIC_FLAG_INIT; // false; true = locked; false = unlocked

int resource = 1; // Resource

unsigned int readers = 0; // number of readers accessing resource

void *reader(void *reader_index)
{
    /*
       wait() decrements (locks) the semaphore pointed to by sem.
       If the semaphore's value is greater than zero, then the decrement
       proceeds, and the function returns, immediately.  If the
       semaphore currently has the value zero, then the call blocks
       until either it becomes possible to perform the decrement (i.e.,
       the semaphore value rises above zero), or a signal handler
       interrupts the call.
    */

    wait(&queue_mutex);

    /*
       The mutex object referenced by mutex shall be locked by a call to
       acquire(). If the mutex is already locked by another thread, the calling thread
       shall block until the mutex becomes available.
    */

    acquire(&reader_mutex);
    if (readers == 0)
        acquire(&writer_mutex);
    // increment the number of users.
    readers++;

    /*
       signal() increments (unlocks) the semaphore pointed to by sem.
       If the semaphore's value consequently becomes greater than zero,
       then another process or thread blocked in a wait() call will
       be woken up and proceed to lock the semaphore.
    */

    signal(&queue_mutex);
    release(&reader_mutex);

    printf("Reader %d reads resource as %d\n",*((int *)reader_index),resource);

    acquire(&reader_mutex);
    readers--;
    if( readers == 0 )
        release(&writer_mutex);
    release(&reader_mutex);

    return reader_index;
}

void *writer(void* writer_index)
{
    wait(&queue_mutex);
    acquire(&writer_mutex);
    signal(&queue_mutex);

    resource  = pow(2,*((int *)writer_index));
    printf("Writer %d modifies resource as %d\n",*((int *)writer_index),resource);

    release(&writer_mutex);

    return writer_index;
}

int main()
{
    // Declaring pthreads or posix threads
    // we are having 10 readers and 10 writers in our system.

    pthread_t readers[10], writers[10];

    // Initialization of queue semaphore
    init(&queue_mutex, 1);

    //num_labels for writers amd readers
    int num_labels[10] = {1,2,3,4,5,6,7,8,9,10};

    /*
        pthread_create: used to create a new thread
        Syntax:        int pthread_create(pthread_t * thread, 
                        const pthread_attr_t * attr, 
                        void * (*start_routine)(void *), 
                        void *arg);
        Parameters:
            thread: pointer to an unsigned integer value that returns the thread id of the thread created.
            attr: pointer to a structure that is used to define thread attributes like detached state, scheduling policy, stack address, etc. Set to NULL for default thread attributes.
            start_routine: pointer to a subroutine that is executed by the thread. The return type and parameter type of the subroutine must be of type void *. The function has a single attribute but if multiple values need to be passed to the function, a struct must be used.
            arg: pointer to void that contains the arguments to the function defined in the earlier argument
    */

    for(int i = 0; i < 10; i++) {
        pthread_create(&readers[i], NULL, reader, (void *)&num_labels[i]);
        pthread_create(&writers[i], NULL, writer, (void *)&num_labels[i]);
    }

    /*
       The pthread_join() function waits for the thread specified by
       thread to terminate.  If that thread has already terminated, then
       pthread_join() returns immediately.  The thread specified by
       thread must be joinable.
    */

    for(int i = 0; i < 10; i++) {
        pthread_join(readers[i], NULL);
        pthread_join(writers[i], NULL);
    }
    
    return 0;
}