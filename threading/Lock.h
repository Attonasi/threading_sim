#ifndef FRAMEWORK_LOCK_H
#define FRAMEWORK_LOCK_H

#include "Thread.h"

// The following functions are implemented for your use as a student, you are not required to implement them,
// just use them as needed.

/**
 * Creates a lock, the lock is a UUID that is a unique string.
 * @return A unique string that represents a lock.
 */
const char* createLock();

/**
 * Locks a given lock, if this lock is locked any thread attempting to lock this lock will wait until the other thread
 * unlocks this lock.  This can result in a deadlock if two threads end up waiting on each other.  If a thread locks a
 * lock, it is responsible for unlocking when it is done.
 * @param lockId The lock id of the lock to be locked.
 * @return true if successful, false otherwise
 */
bool lock(const char* lockId);

/**
 * Unlocks a given lock, this allows another thread requesting the lock to receive the lock.  This function must be called
 * if a lock is locked, otherwise another thread could be blocked.
 * @param lockId The lock id of the lock to be unlocked.
 * @return true if successful, false otherwise
 */
bool unlock(const char* lockId);

/**
 * Destroys a given lock, this lock can no longer be used.
 * @param lockId The lock id of the lock to be destroyed.
 */
void destroyLock(const char* lockId);

/**
 * Returns true if this lock is currently held by any thread.  This will not tell you which thread currently holds
 * the lock.  Maintaining that mapping will fall on the student.
 * @param lockId  The lock id of the lock to be checked.
 * @return true if successful, false otherwise
 */
bool isLocked(const char* lockId);

/**
 * Returns true if this lock currently exists regardless of state.  If the lock provided was created but never destroyed,
 * this will return true.  If the lock id does not exist it will return false.
 * @param lockId The lock id of the lock to be checked.
 * @return true if lock exists, false otherwise.
 */
bool lockExists(const char* lockId);


// The functions below here are functions that are called by the framework when an event happens.  You can use them
// to modify your state when a lock is manipulated.

/**
 * This function is called right after createLock in order to notify anyone that is interested in keeping state of locks.
 * Primarily this can be used for initializing any structures needed to map locks to threads.
 * @param lockId The lock id of the lock that was created.
 */
void lockCreated(const char* lockId);

/**
 * This function is called when an attempt is made on a lock but has not yet been locked.  This function is called
 * syncrhonously during the locking process so you can be guaranteed the lock will not be acquired by the given thread
 * while in this function.
 * @param lockId The lock id of the lock attempted.
 * @param thread The thread that attempted the lock.
 */
void lockAttempted(const char* lockId, Thread* thread);

/**
 * This function is called when a lock is acquired.  This function is called syncrhonously during the locking process
 * so you can be guaranteed the lock will not be released by the given thread while in this function.
 * @param lockId The lock id of the lock acquired.
 * @param thread The thread that acquired the lock.
 */
void lockAcquired(const char* lockId, Thread* thread);

/**
 * This function is called when acquiring a lock fails.  This function is called syncrhonously during the locking process.
 * @param lockId The lock id of the lock acquired.
 * @param thread The thread that failed to acquire the lock.
 */
void lockFailed(const char* lockId, Thread* thread);

/**
 * This function is called when a lock is released.  This function is called syncrhonously during the locking process
 * so you can be guaranteed the lock will not be acquired again by the given thread while in this function.
 * @param lockId The lock id of the lock released.
 * @param thread The thread that acquired the lock.
 */
void lockReleased(const char* lockId, Thread* thread);

// The function below is required to be implemented, it returns the thread that is currently holding the lock.

/**
 * Returns a pointer to the thread that is currently holding the given lock.
 * @param lockId The lock id of the lock we are interested in.
 * @return A pointer to the thread holding the given lock.
 */
Thread* getThreadHoldingLock(const char* lockId);

#endif //FRAMEWORK_LOCK_H
