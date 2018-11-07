#include "goodStuff.h"
#include "testHelper.h"

Map* LockMap = NULL;
Lock* lockKeyList = NULL;
int lockCounter = 0;

LockWrap* createLockWrap() {
    LockWrap* L = (LockWrap*)malloc(sizeof(LockWrap));
    L->ownerThreadKey = NULL;
    memset(L->threadsWaitingForMe, 0, MAX_WAITING_LIST);
    L->threadsWaitingLen = 0;

    return L;
}

void freeLockWrap(LockWrap* L) {
    printSomething("Start free lock wraps ~14 lock.cpp", "nothing", NO_VAL, NO_VAL);
    free(L);
}

void lockCreated(const char* lockId){
    printSomething("Started lockCreated ~380 ", "(lockId)", NO_VAL, NO_VAL);

    addToList(lockKeyList, (void*)lockId);
    lockCounter++;

    addToList(lockKeyList, (void*)lockId);
    lockCounter++;

    LockWrap* newLock = createLockWrap();
    PUT_IN_MAP(Lock*, LockMap, lockId, newLock);
}

void lockAttempted(const char* lockId, Thread* thread){

    LockWrap* L = (LockWrap*)GET_FROM_MAP(Lock*, LockMap, lockId);
    ThreadWrap* T = (ThreadWrap*)GET_FROM_MAP(void*, ThreadMap, thread->arg);

    printSomething("In lock attempted", (char*)lockId, NO_VAL, NO_VAL);
    printThread("In lock attempted ~ 30 lock.cpp ", thread);

    if (L->ownerThreadKey == NULL) {
        printSomething("The lock being locked has no owner", "~62 lock.cpp", NO_VAL, NO_VAL);
    } else {
        lockFailed(lockId, thread);
    }
}

void lockAcquired(const char* lockId, Thread* thread){
    printSomething("The lock being aquired by(name) (pri) (tick) ",
            thread->name, thread->priority, getCurrentTick());

    LockWrap* L = (LockWrap*)GET_FROM_MAP(Lock*, LockMap, lockId);
    ThreadWrap* T = (ThreadWrap*)GET_FROM_MAP(void*, ThreadMap, thread->arg);
    L->ownerThreadKey = thread->arg;
    T->lockWrap = L;
}

void lockFailed(const char* lockId, Thread* thread){

    LockWrap* L = (LockWrap*)GET_FROM_MAP(Lock*, LockMap, lockId);

    printSomething("Start lockFailed. !!!!!!!!!!!!!!!!!!!! ~58 lock.cpp (name) (pri) (tick) ",
                   thread->name, thread->priority, getCurrentTick());

    L->threadsWaitingForMe[L->threadsWaitingLen++] = thread->arg;

    ThreadWrap* T = (ThreadWrap*)GET_FROM_MAP(void*, ThreadMap, thread->arg);
    T->waiting = WAITING;
    T->lockWrap = L;
}

void lockReleased(const char* lockId, Thread* thread){
    printSomething("Start lockReleased ~68 lock.cpp (name) (pri) (tick) ",
                   thread->name, thread->priority, getCurrentTick());

    LockWrap* L = (LockWrap*)GET_FROM_MAP(Lock*, LockMap, lockId);
    ThreadWrap* T = (ThreadWrap*)GET_FROM_MAP(void*, ThreadMap, thread->arg);
    T->lockWrap = NULL;

    removeFromReadyMap(T);
    T->thread->priority = T->thread->originalPriority;
    insertIntoReadyList(T);

    for(int i = 0; i < L->threadsWaitingLen; i++) {
        ThreadWrap* temp = (ThreadWrap*)GET_FROM_MAP(void*, ThreadMap, L->threadsWaitingForMe[i]);

        temp->waiting = NOT_WAITING;
        temp->lockWrap = NULL;
    }

    L->ownerThreadKey = NULL;
}