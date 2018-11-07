#ifndef PROJECT1_THREADING_TESTHELPER_H
#define PROJECT1_THREADING_TESTHELPER_H

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <stdio.h>
#include <malloc.h>
#include "gtest/gtest.h"

#include <Thread.h>
#include <Logger.h>
#include <Lock.h>
#include <Map.h>
#include "List.h"


typedef struct ThreadCallbackInfo {
    char* threadName;
    void* (*func)(void*);
    void* arg;
    int pri;
} ThreadCallbackInfo;

typedef struct Multiply {
    int val;
    int multiplier;
    int answer;
} Multiply;

typedef struct PriorityInfo {
    char* threadName;
    int priority;
} PriorityInfo;

typedef struct PriorityInfoList {
    PriorityInfo* list;
    int size;
    ThreadCallbackInfo** tcbiList;
    bool* cont;
    int* numThreadsStarted;
    pthread_mutex_t varMemLock;
} PriorityInfoList;

typedef struct SleepInfo {
    int tickSleepStarted;
    int ticksToSleep;
    int tickWokenUp;
} SleepInfo;

typedef struct ThreadLockInfo {
    Thread thread;
    bool lockHeld;
} ThreadLockInfo;

typedef struct DonationInfo {
    ThreadLockInfo* threadOrder;
    int* numThreadsFinished;
    const char* lock;
    pthread_mutex_t* varMemLock;
    bool* cont;
    ThreadCallbackInfo* tcbi;
} DonationInfo;


void* multiply(void *arg);
void* recordThreadPriority(void* arg);
void* sleepTest(void* arg);
void* simpleLock(void* arg);
void* donationPriority(void* arg);
void* setMyPriorityTest(void* arg);


#endif //PROJECT1_THREADING_TESTHELPER_H
