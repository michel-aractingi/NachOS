#include "copyright.h"
#include "system.h"
#include "userthread.h"
#include "thread.h"
#include "addrspace.h"
#include "synch.h"
// for test
#include "syscall.h"

 
int do_UserThreadCreate(int f, int arg) {
  ThreadUser* threadUser = new ThreadUser;
  threadUser->f = f;
  threadUser->arg = arg;
  Thread* newThread = currentThread->space->InitUserThread();
  newThread->Fork(StartUserThread, (int)threadUser); 
  return newThread->Tid;
}

int do_UserThreadExit() {
  currentThread->space->semJoin[currentThread->numberOfThread].semaphore->V();
  exitLock->P();
   //fprintf(stdout, "Thread %d exited in proc %d\n",currentThread->Tid,currentThread->space->GetPid());
  if(currentThread->space->isLast()){
      if(machine->isLast()){
          interrupt->Halt();
}
  machine->ExitThread();
}
  currentThread->space->ExitThread();
  currentThread->space->bitmap->Clear(currentThread->numberOfThread);
  currentThread->space->Removeid(currentThread->numberOfThread);
  exitLock->V();
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
void do_UserThreadJoin( int id){
   int ThreadNum = currentThread->space->Findid(id);
   //fprintf(stdout, "thread %d waiting on thread %d with bindex %d\n",currentThread->Tid,id,ThreadNum); 
   if(ThreadNum!=-1){
   currentThread->space->semJoin[ThreadNum].semaphore->P();
   //fprintf(stdout, "Join Successful\n");
}
}

void createProc(int lol) {
  // for (int i=0; i>lol; i++) {}
  currentThread->space->RestoreState();
  currentThread->space->InitRegisters();

  machine->Run();
    printf("found\n");
}

void do_ForkExec(char *f) {

  OpenFile* exec = fileSystem->Open (f,1);
  //fprintf(stdout,"reached\n"); 
  AddrSpace* space = new AddrSpace (exec);
  Thread* proc = new Thread(f);
  machine->forklock->P();
  proc->space = space;
  proc->Tid = proc->space->giveTid();
  proc->numberOfThread = proc->space->bitmap->Find();
  proc->space->Addid(proc->Tid,proc->numberOfThread);
  proc->space->semJoin[proc->numberOfThread].semaphore = new Semaphore ("Join Semaphore",0);
  machine->AddThread();
  machine->forklock->V();
  //space->setChild();
  //space->AddChild();
  proc->Fork(createProc, 0);
  //proc->space->AddThread();
}
