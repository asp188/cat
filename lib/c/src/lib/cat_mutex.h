//
// Created by Terence on 2018/8/14.
//

#ifndef CCAT_CAT_MUTEX_H
#define CCAT_CAT_MUTEX_H

#ifdef WIN32

typedef LPCRITICAL_SECTION CATCRITICALSECTION;
#define INVALID_CRITSECT NULL

static inline CATCRITICALSECTION CATCreateCriticalSection()
{
    CATCRITICALSECTION cs = (CRITICAL_SECTION *)malloc(sizeof(CRITICAL_SECTION));
    //assert(cs != INVALID_CRITSECT);
    if (0)
    {
        InitializeCriticalSection(cs);
    }
    else
    {
        InitializeCriticalSectionAndSpinCount(cs, 4000);
    }
    return cs;
}

static inline void CATDeleteCriticalSection(CATCRITICALSECTION cs) {
    if (cs != INVALID_CRITSECT) {
        DeleteCriticalSection(cs);
        free(cs);
        cs = 0;
    }
}

#define CATCS_ENTER(cs) EnterCriticalSection(cs)
#define CATCS_LEAVE(cs) LeaveCriticalSection(cs)

#define MUTEX CRITICAL_SECTION
#define MUTEX_LOCK(mutex) EnterCriticalSection(&mutex)
#define MUTEX_UNLOCK(mutex) LeaveCriticalSection(&mutex)
#define MUTEX_INIT(mutex) InitializeCriticalSection(&mutex)
#define MUTEX_DESTROY(mutex) DeleteCriticalSection(&mutex)

#elif defined(__linux) || defined(__APPLE__)

#include <pthread.h>
#include <stdlib.h>
#include <assert.h>

#ifndef PTHREAD_MUTEX_RECURSIVE_NP
#define PTHREAD_MUTEX_RECURSIVE_NP PTHREAD_MUTEX_RECURSIVE
#endif

typedef pthread_mutex_t *CATCRITICALSECTION;
#define INVALID_CRITSECT NULL

static inline CATCRITICALSECTION CATCreateCriticalSection() {
    CATCRITICALSECTION cs = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
    assert(cs != INVALID_CRITSECT);

    pthread_mutexattr_t mutexattr;
    pthread_mutexattr_init(&mutexattr);
    pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(cs, &mutexattr);
    pthread_mutexattr_destroy(&mutexattr);

    return cs;
}

static inline void CATDeleteCriticalSection(CATCRITICALSECTION cs) {
    if (cs != INVALID_CRITSECT) {
        pthread_mutex_destroy(cs);
        free(cs);
        cs = 0;
    }
}

#define CATCS_ENTER(cs) pthread_mutex_lock(cs)
#define CATCS_LEAVE(cs) pthread_mutex_unlock(cs)

#define MUTEX pthread_mutex_t
#define MUTEX_LOCK(mutex) pthread_mutex_lock(&mutex)
#define MUTEX_UNLOCK(mutex) pthread_mutex_unlock(&mutex)
#define MUTEX_INIT(mutex) pthread_mutex_init(&mutex, NULL)
#define MUTEX_DESTROY(mutex) pthread_mutex_destroy(&mutex)

#endif

#endif //CCAT_CAT_MUTEX_H
