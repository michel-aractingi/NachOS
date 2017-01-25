// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "userthread.h"
//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------
void
copyStringFromMachine ( int from , char *to  , int size){
     int i;
     int ch;
	for(i=0;i<size;i++)
	{
		
	if(!machine->ReadMem(from + i, 1 , &ch)){fprintf(stdout,"error");}
		to[i] = (char) ch;
		//fprintf(stdout,"%d\n",ch);
	}

to[i] = '\0';

}
void
copyStringToMachine ( int addr , char *from , int size){
	int i;
	for( i = 0; i< size ; i++){
		if (from[i] == '\0') break;
		machine->WriteMem(addr + i ,1,from[i]);
}
}

void
ExceptionHandler(ExceptionType which)
{
  char buff[MAX_STRING_SIZE];
  //buff = NULL;
  int type = machine->ReadRegister(2);
  if (which == SyscallException) {
    switch (type) {
    case SC_Halt: {
      DEBUG('a', "Shutdown, initiated by user program.\n");
//      if (currentThread->space->GetNumOfThreads() == 0 && childCount == 0)
        interrupt->Halt();
      break;
    }

    case SC_PutChar: {
      DEBUG('a', "PutChar exception.\n");
      ioLock->Acquire();
      synchconsole->SynchPutChar(machine->ReadRegister(4));
      ioLock->Release();
      break;
    }
    case SC_PutString: {
      DEBUG('a', "PutString exception.\n");
	//fprintf(stdout,"R4 %d\n",machine->ReadRegister(4));
        ioLock->Acquire();
      	copyStringFromMachine(machine->ReadRegister(4),buff,MAX_STRING_SIZE);
	//fprintf(stdout,"buffer %s\n",buff);
      	synchconsole->SynchPutString(buff);// fprintf(stdout,"out of put\n");
        ioLock->Release();
      break;
    }
    case SC_Exit: {
     DEBUG('a', "Exit process terminated\n");
     fprintf(stdout, "Thread %d exited in proc %d\n",currentThread->Tid,currentThread->space->GetPid());
     exitLock->P();
     if(currentThread->space->isLast())
     {
     if(machine->isLast()){
          interrupt->Halt();
    }
        machine->ExitThread();
    }
     currentThread->space->bitmap->Clear(currentThread->numberOfThread);
     currentThread->space->Removeid(currentThread->numberOfThread);
     currentThread->space->ExitThread(); 
     exitLock->V();
     currentThread->Finish();

      break;

 }
    case SC_GetChar: {
      DEBUG('a', "GetChar exception.\n");
      machine->WriteRegister(2,(int)synchconsole->SynchGetChar());
      break;
    }
    case SC_GetString: {
      DEBUG('a', "GetString exception.\n");
      synchconsole->SynchGetString(buff,MAX_STRING_SIZE);
      copyStringToMachine (machine->ReadRegister(4),buff,MAX_STRING_SIZE);
      break;
    }
    case SC_PutInt: {
          DEBUG('a', "PutInt Exception\n");
          int value = machine->ReadRegister(4);
          synchconsole->SynchPutInt(value);
          break;
    }
   
     case SC_GetInt: {
          DEBUG('a', "GetInt Exception\n");
	  int a=0;
	  int* n;
          n=&a;
	  synchconsole->SynchGetInt(n);
	  //`fprintf(stdout,"buffe22r %d\n",*n);
	  machine->WriteMem(machine->ReadRegister(4),sizeof(int), *n);
        
	break;
    }

    case SC_UserThreadCreate: {
      DEBUG('a', "UserThreadCreate exception.\n");
      int ret = do_UserThreadCreate(machine->ReadRegister(4),
                           machine->ReadRegister(5));
      machine->WriteRegister(2,ret);
      break;
    }
    case SC_UserThreadExit: {
      DEBUG('a', "UserThreadCreate exception.\n");
      do_UserThreadExit();
      break;
    }
    case SC_UserThreadJoin: {
      DEBUG('a', "UserThreadJoin exception.\n");
    //  fprintf(stdout,"r4 %d\n",machine->ReadRegister(4));
      do_UserThreadJoin(machine->ReadRegister(4));
      break;
    }
    case SC_ForkExec: {
      DEBUG('a', "ForkExec exception.\n");
      copyStringFromMachine(machine->ReadRegister(4),buff,MAX_STRING_SIZE);
      do_ForkExec(buff);
      break;

    }
#ifdef FILESYS
        case SC_Create:{
            copyStringFromMachine(machine->ReadRegister(4),buff,MAX_STRING_SIZE);
            if(buff == NULL){
                break;
            }
            fileSystem->Create(buff,0,currentThread->currentSector);
            break;
        }
        case SC_Read:{

            copyStringFromMachine(machine->ReadRegister(4),buff,MAX_STRING_SIZE);
            if(buff != NULL){
                int size = machine->ReadRegister(5);

                OpenFileId id = machine->ReadRegister(6);
                if(id == ConsoleOutput){
                    synchconsole->SynchGetString(buff,MAX_STRING_SIZE);
                    copyStringToMachine (machine->ReadRegister(4),buff,MAX_STRING_SIZE);
                }
                else{
                    ioLock->Acquire();
                    OpenFile *f = currentThread->fileTable->Resolve(id);
                    int numRead = 0;
                    if(f!=NULL){
                        numRead = f->Read(buff,size);
                        printf("%d : %d : %s\n",size,numRead,buff);

                    }
                    ioLock->Release();
                    copyStringToMachine(machine->ReadRegister(4),buff,numRead);
                    machine->WriteRegister(2,numRead);
                }
            }
            break;

        }
        case SC_Write:{
            copyStringFromMachine(machine->ReadRegister(4),buff,MAX_STRING_SIZE);
            if(buff != NULL){
                int size = machine->ReadRegister(5);
                OpenFileId id = machine->ReadRegister(6);
                if(id == ConsoleOutput){
                    synchconsole->SynchPutString(buff);
                }
                else{
                    ioLock->Acquire();
                    OpenFile *f = currentThread->fileTable->Resolve(id);
                    if(f!=NULL){
                        f->Write(buff,size);
                    }
                    ioLock->Release();
                }

            }
            break;
        }
        case SC_ChangeDirectory:{
            copyStringFromMachine(machine->ReadRegister(4),buff,MAX_STRING_SIZE);
            int sector = fileSystem->ChangeDir(buff,currentThread->currentSector);
            currentThread->currentSector = sector;
            machine->WriteRegister(2,sector);
            break;
        }
        case SC_Open:{
            copyStringFromMachine(machine->ReadRegister(4),buff,MAX_STRING_SIZE);
            if(buff != NULL){
                OpenFile *f = fileSystem->Open(buff,currentThread->currentSector);
                if(f != NULL){
                    OpenFileId id = currentThread->fileTable->Insert(f);
                    machine->WriteRegister(2,id);
                }
            }
            else {
                machine->WriteRegister(2, -1);
            }
            break;
        }
        case SC_Close:{
            OpenFileId id = machine->ReadRegister(4);
            currentThread->fileTable->Remove(id);
            break;
        }
        case SC_MakeDirectory:{
            copyStringFromMachine(machine->ReadRegister(4),buff,MAX_STRING_SIZE);
            //printf("Created from : %d\n", currentThread->space->currentSector);
            if(fileSystem->MakeDir(buff,0, currentThread->currentSector)){
                machine->WriteRegister(2,1);

            }
            else{
                machine->WriteRegister(2,-1);
            }
            break;
        }
 	case SC_Remove:{
            copyStringFromMachine(machine->ReadRegister(4),buff,MAX_STRING_SIZE);
	    fileSystem->Remove(buff, currentThread->currentSector); 
            break;
        }    
#endif //FILESYS
    default: {
      printf("Unexpected user mode exception %d %d\n", which, type);
      ASSERT(FALSE);
    }

    }
  }
 
  UpdatePC();
}
