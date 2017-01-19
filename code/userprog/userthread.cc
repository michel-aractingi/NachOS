#include "copyright.h"
#include "system.h"
#include "userthread.h"
#include "thread.h"
#include "addrspace.h"
#include "synch.h"
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
  newThread->space->semJoin[newThread->numberOfThread].semaphore = new Semaphore ("Join Semaphore",0);
  newThread->space->semJoin[newThread->numberOfThread].counter = 0;
 // fprintf(stdout,"counter %d\n", newThread->space->semJoin[newThread->numberOfThread].counter);
  bit_lock->V();
  if(newThread->numberOfThread == -1) return -1;
  newThread->Fork(StartUserThread, (int)threadUser);
  return 0;
}

int do_UserThreadExit() {
  if(currentThread->space->semJoin[currentThread->numberOfThread].counter != 0){
  currentThread->space->semJoin[currentThread->numberOfThread].semaphore->V();
}
  exitLock->P();
  fprintf(stdout, "Thread %d exited \n",currentThread->numberOfThread);
  if(currentThread->space->isLast()){
      if(machine->isLast()){
          interrupt->Halt();
}
  machine->ExitThread();
}
  currentThread->space->ExitThread();
  exitLock->V();
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
    currentThread->space->semJoin[ThreadNum].counter++;
    currentThread->space->semJoin[ThreadNum].semaphore->P();
    fprintf(stdout, "Join Successful\n");
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
  machine->AddThread();
  //space->setChild();
  //space->AddChild();
  proc->Fork(createProc, 0);
  //proc->space->AddThread();
}
