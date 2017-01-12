#include "copyright.h"
#include "system.h"
#include "thread.h"
#include "addrspace.h"
#include "synch.h"

#include "userthread.h"

// for test
#include "syscall.h"


int do_UserThreadCreate(int f, int arg) {

//  fprintf(stdout, "creating!!!!!!!!!!!!!!!!!!!!!\n");
  ThreadUser* threadUser = new ThreadUser;
  threadUser->f = f;
  threadUser->arg = arg;

  Thread* newThread = new Thread("user_thread");

  Semaphore *bit_lock = new Semaphore("Bit Lock Semaphore", 1);
  newThread->space = currentThread->space;
  bit_lock->P();
  newThread->space->AddThread();
  newThread->numberOfThread = newThread->space->bitmap->Find();
  bit_lock->V();
  if(newThread->numberOfThread == -1) return -1;
  newThread->Fork(StartUserThread, (int)threadUser);
  return 0;
}

int do_UserThreadExit() {
  currentThread->space->ExitThread();
  fprintf(stdout, "Thread %d exited \n",currentThread->numberOfThread);
  currentThread->space->bitmap->Clear(currentThread->numberOfThread);
  currentThread->Finish();
  

  return 0;
}

void StartUserThread(int f) {
  ThreadUser* threadUser = (ThreadUser*)f;
  currentThread->space->InitRegisters();
  currentThread->space->RestoreState();

  machine->WriteRegister(PCReg, threadUser->f);
  machine->WriteRegister(NextPCReg, (threadUser->f)+4);
  machine->WriteRegister(4, threadUser->arg);

  //fprintf(stdout,"argument %d thread %d\n",threadUser->arg,currentThread->numberOfThread);


  machine->WriteRegister(StackReg, machine->ReadRegister(StackReg) - 2*PageSize - currentThread->numberOfThread*PageSize);

  machine->Run();
}
void do_UserThreadJoin( int ThreadNum){
    fprintf(stdout, "waiting on thread %d\n",ThreadNum);
   
    while(currentThread->space->bitmap->Test(ThreadNum))
        {
 	currentThread->Yield();
     //   fprintf(stdout, "Waiting on thread to perform join\n");
        }
    fprintf(stdout, "Join Successful\n");
    do_UserThreadExit();
}

void createProc(int lol) {
  // for (int i=0; i>lol; i++) {}
  currentThread->space->RestoreState();
  currentThread->space->InitRegisters();
  machine->Run();
}

void do_ForkExec(char *f) {
  OpenFile* exec = fileSystem->Open (f);
  AddrSpace* space = new AddrSpace (exec);
  Thread* proc = new Thread(f);

  proc->space = space;
  proc->Fork(createProc, 0);

}
