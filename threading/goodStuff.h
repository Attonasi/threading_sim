//
// Created by collin on 10/9/18.
//

#ifndef PROJECT1_THREADING_GOODSTUFF_H
#define PROJECT1_THREADING_GOODSTUFF_H

#include "Thread.h"
#include "List.h"
#include "Map.h"
#include "Lock.h"
#include "Logger.h"

#define READY 1
#define SLEEP 0
#define DONE 0
#define NOT_DONE 1
#define LOG_BUFFER 1024
#define NO_VAL 42
#define WAITING 1
#define NOT_WAITING 0
#define MAX_WAITING_LIST 5
#define EMPTY -1

// const char sucks to type and these are apt descriptions of the objects
typedef const char Lock;
typedef const char Map;
typedef const char List;

extern List* sleepList;

extern Map* LockMap ;
extern Map* ThreadMap;
extern Map* ThreadReadyListMap;
extern int lockCounter;
extern List* lockKeyList;

/**LockWrap: Struct that holds information useful to the maintenance of
 * a lock.
 *
 * Signature: createLockWrap: () => LockWrap*
 *
 * Thread* owner: Thread that currently holds this lock.
 * void* waitingForMeMapKeys: If this lock is held all of the threads that tried
 * to aquire this lock and failed will be here.
 * int numWaitingForMe: Number of threads in waiting for me mapkeys.
 *
 * @return pointer to a lockwrap that can be placed as a value with the key
 * being the lock itself in a map.
 */
typedef struct LockWrap {
    void* ownerThreadKey;
} LockWrap;

LockWrap* createLockWrap();

void freeLockWrap(LockWrap* L);
void freeLockWraps(char* type, void* val);

extern int lockCounter;
extern List* lockKeyList;

/**LockWrap: Struct that holds information useful to the maintenance of
 * a lock.
 *
 * Signature: createLockWrap: () => LockWrap*
 *
 * Thread* owner: Thread that currently holds this lock.
 * void* waitingForMeMapKeys: If this lock is held all of the threads that tried
 * to aquire this lock and failed will be here.
 * int numWaitingForMe: Number of threads in waiting for me mapkeys.
 *
 * @return pointer to a lockwrap that can be placed as a value with the key
 * being the lock itself in a map.
 */
typedef struct LockWrap {
    void* ownerThreadKey;
    void* threadsWaitingForMe[MAX_WAITING_LIST];
    int threadsWaitingLen;
} LockWrap;

LockWrap* createLockWrap();

void freeLockWrap(LockWrap* L);
void freeLockWraps(char* type, void* val);

/**ThreadWrap holds information useful to the management and operation
 * of a Thread*.
 *
 * Signature: createThreadWrap: Thread* => ThreaWrap*
 *
 * Thread* T: Thread* maintained in the ThreadWrap
 * readListIndex: If the Thread that owns this lock is on the readyList
 * it's index will be here.
 * sleepListIndex: If the Thread is in the sleep list the index will be here.
 * Lock* lock if the Thread* in this wrap holds a lock it is listed here.
 * int wakeTick if the thread is put to sleep the tick it is supposed to
 * wake up is maintained here.
 * int waiting this is a flag that tells nextThreadToRun whether or not this
 * thread is waiting for nother thread to release a lock. If 1 true else if
 * 0 it is not waiting.
 *
 * @return ThreadWrap* pointer to a ThreadWrap that will be placed as the
 * value in the ThreadMap.
 *
 * Currently the Map key is the void* for arg in Thread* T.
 */
typedef struct ThreadWrap {
    Thread* thread;
    int readyListIndex;
    int sleepListIndex;
    int wakeTick;
    int waiting;
    LockWrap* lockWrap;
} ThreadWrap;

ThreadWrap* createThreadWrap(Thread* T);

void freeThreadWrap(ThreadWrap* T);

/**This function goes through a Map of ready lists and frees the threadWraps and
 * destroys the lists after they are empty.
 *
 * Signature: freeThreadReadyMap: () => (void)
 *
 * @param readyap Map of ready lists that needs to be cleared and emptied out.
 */
void freeThreadReadyMap();

/**Print functions for test purposes
 *
 * @param descr line added to print describing where the print was called,
 * why it was called and what the values are.
 *
 * Signature: printSomething: char*, char*, int, int => (void)
 *
 * Signature: printThread: char*, Thread* => (void)
 *
 * Signature: printThreadWrap: char*, ThreadWrap* => (void)
 *
 * key parameter has a similar function if called by the user to descr. Put
 * whatever you want there. If called  * by EXEC_ON_MAP it will print the key.
 *
 * @param name a string or name of object printed
 * @param val1 an int the user wants to know about
 * @param val2 anopther int we want to know about
 * @param T Thread or ThreadWrap depending on which function called.
 */
void printSomething(char* descr, char* name, int val1, int val2);
void printThread(char* descr, Thread* T);
void printThreadWrap(char* key, void* T);

/**InsertIntoSleepList takes a threadWrap and begins it down the path it needs
 * to follow to get into the correct place in the correct list.
 *
 * Signature: insertIntoSleepList: ThreadWrap*, int => (void)
 *
 * insertIntoSleepList is a helper function.
 *
 * @param T The threadwrap going into a list
 */
void insertIntoList(ThreadWrap* T, int typeOfListAdd);
int insertIntoSleepList(ThreadWrap* T);

/**clearSleepList is called every tick and the first item's wakeTick is checked
 * against curentTick.
 *
 * Signature: clearSleepList: int => (void)
 *
 * If currentTick is >= WakeTick it is removed from the sleep list and added back
 * into the readyLists.
 *
 * This list is maintained in sorted order using insertion when adding new members
 * to the list so only the first Thread need be checked. If it is removed the next
 * member is checked. As soon as a wakeTick is > currentTick this function ends.
 *
 * @param currentTick
 */
void clearSleepList(int currentTick);

/**This function checks to see if there is a list at the map position determined
 * by the threads priority.
 *
 * Signature: insertIntoReadyList: ThreadWrap* => (void)
 *
 * @param T ThreadWrap being entered into the map of lists.
 */
void insertIntoReadyList(ThreadWrap* T);

/** getReadyListFromMap is called in nextThreadToRun. It starts at 10 and works to 1
 * representing the priorities of the Threads in the lists. It looks for a List with
 * ThreadWraps in it and passes that List to getFromReadyList.
 *
 * Signature: getReadyListFromMap: () => ThreadWrap*
 *
 * If getFromReadyList returns a ThreadWrap* this is returned.
 *
 * If getFromReadyList returns NULL it moves on to hte next list.
 *
 * Null will be returned if there are no threads that meet hte criteria.
 *
 * @return ThreadWrap* containing the Thread* we need to return in nextThread
 */
ThreadWrap* getReadyListFromMap();

/**getFromReadyList takes a list and returns the first legitimate ThreadWrap in that list.
 *
 * Signature: getFromReadyList: list<List*> => ThreadWrap*
 *
 * If there are more than 1 items in the list the first item is removed from the front and
 * placed at the end of the list to simulate "round robin"
 *
 * @param list List of threadWraps that contain Threads with the same current priority.
 * @return The first legal member of the list or NULL if there are no legal Threads
 */
ThreadWrap* getFromReadyList(List* list);

/**This function takes a list and sets the ThreadWrap->readyIndex or sleepIndex values to
 * the actual index. This is done when the items in a list are reshuffled or removed.
 *
 * Signature: getFromReadyList: list<List*> => ThreadWrap*
 *
 * @param readyOrSleep tells the function which values to reset
 * @param list List of ThreadWraps that needs to be reset
 */
void setListIndexes(List* list, int readyOrSleep);

/**When removing a ThreadWrap from the ready map there is a lot of housekeeping to
 * do. The TW needs to have it's indexes updated and the list needs
 * to be reset. If the list is empty it needs to be destroyed.
 *
 * Signature: removeFromReadyMap: ThreadWrap* = (void)
 *
 * @param T ThreadWrap being removed from the lsit.
 */
void removeFromReadyMap(ThreadWrap* T);

/**donatePriority is called when the waiting flag of the ThreadWrap chosen by nextThreadToRun
 * is chosen is True.
 *
 * donatePriority will look at the lockWrap held by the chosen thread and take the thread that
 * is holding the lock from the wrap. If the thread that holds the lock has a lower priority
 * than the thread that is chosen by nextThreadToRun this function will raise the priority of
 * the thread holding the lock.
 *
 * Signature: donatePriority: ThreadWrap* => (void)
 *
 * @param T ThreadWrap of thread chosen by nextThreadToRun that is trying to take a lock.
 */
void donatePriority(ThreadWrap* T);

#endif //PROJECT1_THREADING_GOODSTUFF_H
