#ifndef __INT_LOCK_H__
#define __INT_LOCK_H__

#include <thread.h>

extern int mutex_glb;
void mutex_glb_wait();

#define intLock mutex_glb_wait();
#define intUnlock mutex_glb = 1;

#endif
