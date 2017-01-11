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
    bit_lock->P();
  newThread->space = currentThread->space;
  newThread->space->AddThread();
  newThread->numberOfThread = newThread->space->bitmap->Find();
    bit_lock->V();
  if(newThread->numberOfThread == -1) return -1;
  //fprintf(stdout, "nuuuuuuuuuuuuuuuum%d\n", currentThread->space->GetNumOfThreads());

  newThread->Fork(StartUserThread, (int)threadUser);
  //fprintf(stdout, "created!!!!!!!!!!!!!!!!!!!!! \n");
  return 0;
}

int do_UserThreadExit() {
  //PutChar('o');
  // aidd space clean
  currentThread->space->ExitThread();
  //fprintf(stdout, "decreeee!!!!!!!!!!!!!!!!!!!!! \n");
  currentThread->space->bitmap->Clear(currentThread->numberOfThread);
  currentThread->Finish();
  

  return 0;
}

void StartUserThread(int f) {
 // fprintf(stdout, "start user th!!!!!!!!!!!!!!!!!!!!!\n");
  ThreadUser* threadUser = (ThreadUser*)f;
  currentThread->space->InitRegisters();
  currentThread->space->RestoreState();

  machine->WriteRegister(PCReg, threadUser->f);
  machine->WriteRegister(NextPCReg, (threadUser->f)+4);


  machine->WriteRegister(4, threadUser->arg);

  // TODO: add different spaces for different threads
  //machine->WriteRegister(StackReg, 0);
  machine->WriteRegister(StackReg, machine->ReadRegister(StackReg) - 2*PageSize - currentThread->numberOfThread*PageSize);
    int value = machine->ReadRegister(StackReg);
    printf("%d : ", value);
  machine->Run();
}
