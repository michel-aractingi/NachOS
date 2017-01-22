// addrspace.h 
//      Data structures to keep track of executing user programs 
//      (address spaces).
//
//      For now, we don't keep any information about address spaces.
//      The user level CPU state is saved and restored in the thread
//      executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
// #include "synch.h"
#include "bitmap.h"
#include "synch.h"
#include "thread.h"
#define UserStackSize		1024	// increase this as necessary!
#define NumberOfAreas     8
/*typedef struct SemJoin {
   Semaphore *semaphore;
   int counter;
} SemJoin;*/
class Semaphore;
class Thread;
class SemJoin
{
  public:
 // SemJoin(int n);
  //~SemJoin(); 
  Semaphore *semaphore;
  
};

class AddrSpace
{
  public:
    AddrSpace (OpenFile * executable);	// Create an address space,
    // initializing it with the program
    // stored in the file "executable"
    ~AddrSpace ();		// De-allocate an address space

    void InitRegisters ();	// Initialize user-level CPU registers,
    // before jumping to user code

    void SaveState ();		// Save/restore address space-specific
    void RestoreState ();	// info on a context switch 
    int Findid(int id);  
    void Addid(int id,int num);
    void Removeid(int ThreadNum);
    void AddThread();
    void ExitThread();
    int GetNumOfThreads();
    bool isLast();
    int dothis();
    int giveTid();
    Thread* InitUserThread(); 
    BitMap *bitmap;
    SemJoin *semJoin;
    int GetPid();
  private:
      TranslationEntry * pageTable;	// Assume linear page table translation
    // for now!
    unsigned int numPages;	// Number of pages in the virtual 
    // address space
    unsigned int numOfThreads;  // number of threads within this address space
    unsigned int Tid; 
    int *idToBitmap;
    unsigned int Pid;
};

#endif // ADDRSPACE_H
