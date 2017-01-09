#ifndef USERTHREAD_H
#define USERTHREAD_H

#include "copyright.h"
#include "system.h"
#include "syscall.h"

extern int numOfThreads;

typedef struct {
        int f;
        int arg;
} ThreadUser;


extern int do_UserThreadCreate(int f, int arg);
extern int do_UserThreadExit();
extern void StartUserThread(int f);

#endif