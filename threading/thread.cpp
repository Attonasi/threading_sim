#include "testHelper.h"
#include "goodStuff.h"

List* sleepList = NULL;
Map* ThreadMap = NULL;
Map* ThreadReadyListMap = NULL;

ThreadWrap* createThreadWrap(Thread* thread) {
    ThreadWrap* T = (ThreadWrap*)malloc(sizeof(ThreadWrap) + LOG_BUFFER);

    T->thread = thread;
    T->readyListIndex = EMPTY;
    T->sleepListIndex = EMPTY;
    T->wakeTick = EMPTY;
    T->waiting = NOT_WAITING;
    T->lockWrap = NULL;

    return T;
}

void printSomething(char* descr, char* name, int val1, int val2) {
    char log[LOG_BUFFER];
    sprintf(log, "%s name: %s val1: %d, val2: %d\n", descr, name, val1, val2);
    verboseLog(log);
}

void printThread(char* descr, Thread* T) {
    char log[LOG_BUFFER];
    sprintf(log, "**** THREAD PRINT *****\n Description: %s \n Name: %s\n Priority: %d\n"
                 " Arg: %p (key) \n State: %d \n Orig Priority: %d\n**********\n", descr,
                 T->name, T->priority, T->arg, T->state, T->originalPriority);
    verboseLog(log);
}

void printThreadWrap(char* key, void* P) {
    ThreadWrap* T = (ThreadWrap*)P;
    char log[LOG_BUFFER];
    char line[LOG_BUFFER];

    sprintf(log, "\n******* ThreadWrap Print \n key/loc: %s *****\n", key);

<<<<<<< HEAD
    sprintf(line, " ReadyList index: %d\n SleepList index: %d\n WakeTick: %d\n waiting: %d\n "
                  "Lock: %s\n---------------", T->readyListIndex, T->sleepListIndex,
            T->wakeTick, T->waiting, T->lockWrap);
=======
    sprintf(line, " ReadyList index: %d\n SleepList index: %d\n WakeTick: %d\n "
                  "Waiting: %d\n LockWrap: %p\n---------------\n", T->readyListIndex,
                  T->sleepListIndex, T->wakeTick, T->waiting, T->lockWrap);
>>>>>>> 9dd81ff473048de26ca74c82131f83c26ae8e4f1

    strcat(log, line);
    verboseLog(log);

    printThread("In print ThreadWrap ~48 thread.cpp", T->thread);
}

void freeThreadWrap(ThreadWrap* T) {
    printThreadWrap("Freeing ThreadWrap ~66 thread.cpp", T);

//    destroyThread(T->thread);
    free(T);
}

void destroyThread(Thread* thread) {
    free(thread->name);
    free(thread);
}

void insertIntoList(ThreadWrap* T, int listFlag) {
    if(listFlag == READY) {
        printSomething("Insert into list ~78 (TW name) (readyOrSleep)",
                T->thread->name, listFlag, NO_VAL);

        insertIntoReadyList(T);
        return;
    }

    printSomething("Start insert sleep List~85 (TWname) (SL size)",
            T->thread->name, listSize(sleepList), NO_VAL);

    if(listFlag == SLEEP){
        if (!listSize(sleepList)) {
            addToList(sleepList, T);
            T->sleepListIndex = 0;

            printSomething("Insert at zero ~92 (name) (SLsize)",
                           T->thread->name, listSize(sleepList), NO_VAL);
            return;
        }

        int addAtEnd = insertIntoSleepList(T);

        if (addAtEnd) {
            T->sleepListIndex = listSize(sleepList);
            addToList(sleepList, T);

            printSomething("add end of sleepList ~102 (name) (SL size) (T SL index)",
                           T->thread->name, listSize(sleepList), T->sleepListIndex);
        }
    }
}

int insertIntoSleepList(ThreadWrap* T) {
    int compareValT, compareValList;

    compareValT = T->wakeTick;

    for (int index = 0; index < listSize(sleepList); index++) {
        ThreadWrap *forTempT = (ThreadWrap *)listGet(sleepList, index);

        printSomething("insert into sleepList ~95 (name) (compareT) (compareL)",
                       T->thread->name, T->wakeTick, forTempT->wakeTick);

        if(T->wakeTick < forTempT->wakeTick) {
            ThreadWrap *recurseAdd = (ThreadWrap *)listGet(sleepList, index);
            removeFromListAtIndex(sleepList, index);

            addToListAtIndex(sleepList, index, T);
            T->sleepListIndex = index;
            insertIntoSleepList(recurseAdd);
            setListIndexes(sleepList, SLEEP);

            printSomething("Inserted into sleeplist ~107 (name) (index) (wakeTick)",
                           T->thread->name, index, T->wakeTick);

            return;
        }
    }

    T->sleepListIndex = listSize(sleepList);
    addToList(sleepList, T);

    printSomething("add end of sleepList ~117 (name) (SL size) (T SL index)",
       T->thread->name, listSize(sleepList), T->sleepListIndex);

}


void insertIntoReadyList(ThreadWrap* T) {
    printSomething("start Insert into readylist ~124 (name) (pri)",
            T->thread->name, T->thread->priority, NO_VAL);

    List* list = (List*)GET_FROM_MAP(int, ThreadReadyListMap, T->thread->priority);

    T->readyListIndex = listSize(list);
    addToList(list, T);
}

ThreadWrap* getReadyListFromMap() {
    ThreadWrap* ret = NULL;

    printSomething("Start get ready list from map ", "~136", NO_VAL, NO_VAL);

    for (int i = 10; i > 0; i--) {
<<<<<<< HEAD
        List* list = (List*)GET_FROM_MAP(int, ThreadReadyListMap, i);

        listSize(list) ? ret = getFromReadyList(list) : ret = NULL;

        ret == NULL ? printSomething("ret is NULL", "~144 (i) (list size)", i, listSize(list)) :
            printSomething("inside if(MAP_CONTAINS) ~145 (thread name) (pri) (i)",
                ret->thread->name, ret->thread->priority, i);
         if(ret != NULL) { return  ret; }
=======
        if(MAP_CONTAINS(int, ThreadReadyListMap, i)) {
            List* list = (List*)GET_FROM_MAP(int, ThreadReadyListMap, i);

            if (listSize(list) > 0) {
                ret = getFromReadyList(list);

                if(ret != NULL) {
                    printSomething("inside if(MAP_CONTAINS) ~186 lsize > 0 (thread name) (pri) (i)",
                                   ret->thread->name, ret->thread->priority, i);
                    return  ret;
                }
            } else {
                printSomething("in get ready list list is empty ~183", "(listsize)", listSize(list), NO_VAL);
            }
        }
>>>>>>> 9dd81ff473048de26ca74c82131f83c26ae8e4f1
    }

    return ret;
}

ThreadWrap* getFromReadyList(List* readyList) {
    printSomething("Start getFromReadyList ~155  ",
            "(list size)", listSize(readyList), NO_VAL);

    ThreadWrap* next = NULL;
    int i = 0;

    while(i < listSize(readyList)) {
        next = (ThreadWrap *)listGet(readyList, i);

        printSomething("Inside while getFromReadyList ~164 (next name (next pri) (next state)",
                next->thread->name, next->thread->priority, next->thread->state);

        if (next->thread->state == TERMINATED) {
            printSomething("Thread terminated ~203 (name) (i index) pri",
                           next->thread->name, i, next->thread->priority);

            removeFromReadyMap(next);

            if (listSize(readyList) < 1) {
                printSomething("Inside list empty get from ready list ~209 ", "(rlist size) ",
                               listSize(readyList), NO_VAL);
                return NULL;
            } else { next = NULL; }
        } else {
<<<<<<< HEAD
            printSomething("Not TERMINATED ~176 (tName) (t rlistindex) (list size)",
=======
            printSomething("inside RUNNING ~216 (tName) (t rlistindex) (list size)",
>>>>>>> 9dd81ff473048de26ca74c82131f83c26ae8e4f1
                    next->thread->name, next->readyListIndex, listSize(readyList));

            if (listSize(readyList) > 1){
                printThreadWrap("round robin called ~180", next);

                removeFromList(readyList, next);
                next->readyListIndex = listSize(readyList);
                addToList(readyList, next);
                setListIndexes(readyList, READY);
            }
            return next;
        }
        i++;
    }
    return next;
}

void removeFromReadyMap(ThreadWrap* T) {
    List* readyList = (List*)GET_FROM_MAP(int, ThreadReadyListMap, T->thread->priority);

    printThreadWrap("Removing TW from readyList ~229", (void*)T);

    removeFromList(readyList, T);
    T->readyListIndex = -1;

    if (listSize(readyList) < 1) {
        printSomething("Inside list empty rfrm ~238 ", "(rlist size) ",
                listSize(readyList), NO_VAL);
        REMOVE_FROM_MAP(int, ThreadReadyListMap, T->thread->priority);
        destroyList(readyList);
    } else {
        setListIndexes(readyList, READY);
    }
}

void clearSleepList(int currentTick) {
    int wakeListNotDone = NOT_DONE;

    printSomething("start clear sleep list ~204 ",
            "(currentTick)", currentTick, NO_VAL);

    while(wakeListNotDone){
        ThreadWrap* T = (ThreadWrap*)listGet(sleepList, 0);

        if(T->wakeTick <= currentTick) {
            insertIntoReadyList(T);

            removeFromListAtIndex(sleepList, 0);
            if (listSize(sleepList)){
                wakeListNotDone = NOT_DONE;
                setListIndexes(sleepList, SLEEP);
            } else {
                wakeListNotDone = DONE;
            }
            printSomething("start clear sleep list ~220 (T name) (Slist size) (wakeListDone)",
                    T->thread->name, listSize(sleepList), wakeListNotDone);

        } else {
            wakeListNotDone = DONE;
        }
    }
}

void setListIndexes(List* list, int readyOrSleep) {
    for (int i = 0; i < listSize(list); i++) {
        ThreadWrap* temp = (ThreadWrap*)listGet(list, i);
        readyOrSleep ? temp->readyListIndex = i : temp->sleepListIndex = i;

        printSomething("set list indexes ~235 (T name) (readyOrSleep) (i)",
                       temp->thread->name, readyOrSleep, i);

    }
}

void donatePriority(ThreadWrap* waitingThreadWrap) {
    ThreadWrap* holdingThreadWrap = (ThreadWrap*)GET_FROM_MAP(void*,
            ThreadMap, waitingThreadWrap->lockWrap->ownerThreadKey);

    if (holdingThreadWrap->thread->priority < waitingThreadWrap->thread->priority) {

        removeFromReadyMap(holdingThreadWrap);
        holdingThreadWrap->thread->priority = waitingThreadWrap->thread->priority;
        insertIntoReadyList(holdingThreadWrap);

        printSomething("donating priority ~251 (Holding T name) (holding pri) (waiting pri)",
                       holdingThreadWrap->thread->name, holdingThreadWrap->thread->priority,
                       waitingThreadWrap->thread->priority);

    }

}

void donatePriority(ThreadWrap* waitingThreadWrap) {
    ThreadWrap* holdingThreadWrap = (ThreadWrap*)GET_FROM_MAP(void*,
            ThreadMap, waitingThreadWrap->lockWrap->ownerThreadKey);

    if (holdingThreadWrap == NULL) {
        printSomething("Priority Donation failed ~292 (waiting name) (pri) (waiting)",
                waitingThreadWrap->thread->name, waitingThreadWrap->thread->priority,
                waitingThreadWrap->waiting);
    } else if (holdingThreadWrap->thread->priority < waitingThreadWrap->thread->priority) {

        removeFromReadyMap(holdingThreadWrap);
        holdingThreadWrap->thread->priority = waitingThreadWrap->thread->priority;
        insertIntoReadyList(holdingThreadWrap);

        printSomething("donating priority ~297 (Holding T name) (holding pri) (waiting pri)",
                       holdingThreadWrap->thread->name, holdingThreadWrap->thread->priority,
                       waitingThreadWrap->thread->priority);
    }
}


Thread* createAndSetThreadToRun(char* name, void* (*func)(void*), void* arg, int pri) {

    printSomething("Start create and run ~262 (name) (pri) (current Tick)", name, pri, NO_VAL);

    Thread* ret = (Thread *)calloc(1, sizeof(Thread));

    ret->name = (char*)calloc(1, strlen(name)+1);
    strcpy(ret->name, name);
    ret->func = func;
    ret->arg = arg;
    ret->priority = pri;
    ret->originalPriority = pri;

    printThread("Create and set line ~298", ret);

    ThreadWrap* T = createThreadWrap(ret);
    PUT_IN_MAP(void*, ThreadMap, ret->arg, T);
    insertIntoReadyList(T);

    printThreadWrap("Create and set thread ~305", (void*)T);

    createThread(ret);

    return ret;
}

Thread* nextThreadToRun(int currentTick) {
    printSomething("Next Thread started ~333 ",

            "(currentTick)", currentTick, NO_VAL);

    if(listSize(sleepList)) { clearSleepList(currentTick);  }

    ThreadWrap* T = getReadyListFromMap();

<<<<<<< HEAD
    if (T->waiting == WAITING) { donatePriority(T); }

    T == NULL ? printSomething("T = NULL ~296 (currentTick)",
            "nothing", currentTick, NO_VAL) :
        printThreadWrap("next Thread to run ~298 ", (void*)T);

    if( T == NULL) {
        nextThreadToRun(getCurrentTick());
    } else {
        T->thread;
=======
    if (ret == NULL) {
        printSomething("ret = NULL ~319 ", "(currentTick)", currentTick, NO_VAL);
        return NULL;
    } else {
        if(ret->waiting == WAITING) { donatePriority(ret); }
        printThreadWrap("next Thread to run ~345 ", (void*)ret);
        return ret->thread;
>>>>>>> 9dd81ff473048de26ca74c82131f83c26ae8e4f1
    }
}

void initializeCallback() {

    sleepList = createNewList();
    lockKeyList = createNewList();

    LockMap = CREATE_MAP(Lock*);
    ThreadMap = CREATE_MAP(void*);
    ThreadReadyListMap = CREATE_MAP(int);

    for(int i = 0; i < 10; i++) {
        List* list = createNewList();
        PUT_IN_MAP(int, ThreadReadyListMap, i+1, (void*)list);
    }
}

void freeThreadReadyMap() {
    ThreadWrap* T = NULL;

    printSomething("Start freeThreadReadyMap ", "~321", NO_VAL, NO_VAL);

    for (int i = 10; i > 0; i--) {
        List* list = (List*)GET_FROM_MAP(int, ThreadReadyListMap, i);

        while (listSize(list)) {
           T = (ThreadWrap*)listGet(list, 0);
           freeThreadWrap(T);
           removeFromListAtIndex(list, 0);
        }
        destroyList(list);
    }
}

void shutdownCallback() {

    printSomething("It's over man! It's OVER!!", "shutdownCallback ~336", NO_VAL, NO_VAL);

    destroyList(sleepList);
    destroyList(lockKeyList);
    freeThreadReadyMap();
}


int tickSleep(int numTicks) {
    Thread* thread = getCurrentThread();
    int cTick = getCurrentTick();

    printSomething("Start tickSleep ~355 (thread name) (current Tick) (numTicks)",
            thread->name, cTick, numTicks);

    ThreadWrap* T = (ThreadWrap*)GET_FROM_MAP(void*, ThreadMap, thread->arg);
    T->wakeTick = cTick + numTicks;
    insertIntoSleepList(T);

    printSomething("tickSleep inserted into sleep list ~363 (name) (wakeTick) (Slist len)",
            T->thread->name, T->wakeTick, listSize(sleepList));

    removeFromReadyMap(T);
    T->readyListIndex = -1;
    stopExecutingThreadForCycle();

    return cTick;
}

void setMyPriority(int priority) {

    printSomething("Start set my priority ~374 ", "(priority)", priority, NO_VAL);

    Thread* thread = getCurrentThread();
    ThreadWrap* T = (ThreadWrap*)GET_FROM_MAP(void*, ThreadMap, thread->arg);

    printThreadWrap("setMyPriority before set ~379", T);

    removeFromReadyMap(T);
    T->thread->priority = priority;
    insertIntoReadyList(T);

    printThreadWrap("setMyPriority after set ~385", T);
}

Thread* getThreadHoldingLock(Lock* lockId) {
    LockWrap* L = (LockWrap*)GET_FROM_MAP(Lock*, LockMap, lockId);
    ThreadWrap* T = (ThreadWrap*)GET_FROM_MAP(void*, ThreadMap, L->ownerThreadKey);

    printSomething("Start getThreadHoldingLock ~388 (lock holder) (pri) (tick) ",

            T->thread->name, T->thread->priority, getCurrentTick());

    return T->thread;
}