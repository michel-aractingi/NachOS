// system.h 
//      All global variables used in Nachos are defined here.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef SYSTEM_H
#define SYSTEM_H

#include "copyright.h"
#include "utility.h"
#include "thread.h"
#include "scheduler.h"
#include "interrupt.h"
#include "stats.h"
#include "timer.h"
#include "../userprog/frameprovider.h"
#include "synch.h"
#include "../userprog/filetable.h"

//buffer size for copy string
#define MAX_STRING_SIZE 1024
#define NumPhysPages 512
// Initialization and cleanup routines
extern void Initialize (int argc, char **argv);	// Initialization,
						// called before anything else
extern void Cleanup ();		// Cleanup, called when
						// Nachos is done.
extern Semaphore *exitLock;
extern Thread *currentThread;	// the thread holding the CPU
extern Thread *threadToBeDestroyed;	// the thread that just finished
extern Scheduler *scheduler;	// the ready list
extern Interrupt *interrupt;	// interrupt status
extern Statistics *stats;	// performance metrics
extern Timer *timer;		// the hardware alarm clock
                                  // number of child processes
#ifdef USER_PROGRAM
#include "machine.h"
#include "synchconsole.h"
class FrameProvider;
extern FrameProvider *PFN;
extern Machine *machine;	// user program memory and registers
extern SynchConsole *synchconsole;
extern Lock *ioLock;               // lock for making Read/Write atomic
extern OpenFileTable *globalFileTable;
#endif

#ifdef FILESYS_NEEDED		// FILESYS or FILESYS_STUB
#include "filesys.h"
extern FileSystem *fileSystem;
#endif

#ifdef FILESYS
#include "synchdisk.h"
#include "bitmap.h"
extern SynchDisk   *synchDisk;
extern Lock *directoryLock;
extern Lock *diskmapLock;
extern Lock *diskLock;
extern OpenFile *vmFile;

extern BitMap *diskMap;			// bitmap for allocating disk sectors
// extern SynchDisk *vmDisk;	// our disk for secondary storage
extern AddrSpace *reversePageTable[];
extern Lock *memLock;

#endif

#ifdef NETWORK
#include "post.h"
extern PostOffice *postOffice;
#endif

#endif // SYSTEM_H
