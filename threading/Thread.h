#ifndef OS_THREADING_CTHREAD_H
#define OS_THREADING_CTHREAD_H

// The following are data structures that are used by functions provided or are needed for your implementation.
/**
 * Represents the state of a thread.
 */
typedef enum State {
    CREATED,
    PAUSED,
    RUNNING,
    TERMINATED
} State;

// Priority values
const int MIN_PRI=1;
const int MAX_PRI=10;
const int DEFAULT_PRI=5;

/**
 * Represents a thread.
 * @param name The name of a thread.
 * @priority The priority of a thread, minimum priority is 1, max is 10, default is 5
 * @func The function to run when starting the thread, this function returns a void* and takes a void*.  This is
 * required because pthread takes this as its argument to start a thread.
 * @param arg The argument to pass to the function that starts the thread.
 * @param state Current state of the thread
 * @param originalPriority Priority thread was created with, used for priority donation.
 */
typedef struct Thread {
    char* name;
    int priority;
    void* (*func)(void*);
    void* arg;
    State state;
    int originalPriority;
} Thread;

// These functions are available for you to to call or used to run the tests.

/**
 * Starts the simulator with an idle thread, threads can be added to be run.
 */
void startSystem();

/**
 * Stops the simulator waiting for all threads to finish before returning.  This is a synchronous operation and blocks
 * until all threads are finished.
 */
void stopSystem();

/**
 * Creates a thread in the simulator so that it can be returned by nextThreadToRun.  This makes the simulator know the
 * thread exists.
 * @param thread The thread to create in the simulator.
 */
void createThread (Thread* thread);

/**
 * Stop executing the current thread for the rest of this cycle. This can be used for implementing sleep by stopping
 * any functionality and pausing the thread until it is ready to be executed again.
 */
void stopExecutingThreadForCycle();

/**
 * Returns a pointer to the thread that is currently running.
 * @return A pointer to the thread that is currently running
 */
Thread* getCurrentThread();

/**
 * Gets the current tick of the "CPU".  The ticks start at 1 nd go up once per tick.  THere is an approximate
 * correlation of 50 microseconds with each tick. This should not be relied on because system calls and OS scheduling
 * affect the simulator as it runs in user space.
 * @return The current tick
 */
int getCurrentTick();

//You are required to implement the functions below.  The tests rely on this to work correctly.

/**
 * The heart of the simulator, this function is called every tick and the thread returned is what runs for that tick.
 * The default implementation is a simple queue that works as first in first out.  You will require significant changes
 * to support sleep and priority.
 * @param currentTick The current tick that the returned thread will run in.
 * @return The thread to run for the current tick.
 */
Thread* nextThreadToRun(int currentTick);

/**
 * This function should prepare a thread to run which at a minimum means building the thread object, calling
 * createThread and adding to some sort of ready list.  This is exactly what the default implementation does.
 * @param name The name of a thread.
 * @param func The function to run when starting the thread, this function returns a void* and takes a void*.  This is
 * required because pthread takes this as its argument to start a thread.
 * @param arg The argument to pass to the function that starts the thread.
 * @param pri The priority of a thread, minimum priority is 1, max is 10, default is 5
 * @return The thread that was set to run.
 */
Thread* createAndSetThreadToRun(char* name, void* (*func)(void*), void* arg, int pri);

/**
 * Tells the current thread to sleep for the number of ticks in the argument.  This should sleep for exactly that
 * number of tick given. This function must return the tick that the sleep was supposed to start as no guarantee can be
 * made on the exact tick that the sleep started.
 * @param numTicks The number of ticks to sleep
 * @return The tick that the sleep is supposed to start.
 */
int tickSleep(int numTicks);

/**
 * Free the memory association with the thread object.
 * @param thread The thread to free memory from.
 */
void destroyThread(Thread* thread);

/**
 * Allow a thread to set its priority to whatever it wants, this can be useful during priority donation and may also
 * have to adjust the ready list so this thread gets run in the current priority on the next tick.
 * @param newPriority
 */
void setMyPriority(int newPriority);

/**
 * This function is called after the simulator is started but before the idle thread starts running to allow you to
 * initialize any objects you need to initialize.
 */
void initializeCallback();

/**
 * This function is called before shutdown of the simulator to do any clean up actions you need to do.
 */
void shutdownCallback();

#endif //OS_THREADING_CTHREAD_H
