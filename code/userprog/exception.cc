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
		
		if(!machine->ReadMem(from + i*4, 1 , &ch)){fprintf(stdout,"error");}
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
#ifndef CHANGED
void
ExceptionHandler (ExceptionType which)
{
    int type = machine->ReadRegister (2);

    if ((which == SyscallException) && (type == SC_Halt))
      {
	  DEBUG ('a', "Shutdown, initiated by user program.\n");
	  interrupt->Halt ();
      }
    else
      {
	  printf ("Unexpected user mode exception %d %d\n", which, type);
	  ASSERT (FALSE);
      }

    // LB: Do not forget to increment the pc before returning!
    UpdatePC ();
    // End of addition
}
#else

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
      if (currentThread->space->GetNumOfThreads() == 0)
        interrupt->Halt();
      break;
    }

    case SC_PutChar: {
      DEBUG('a', "PutChar exception.\n");
      synchconsole->SynchPutChar(machine->ReadRegister(4));
      break;
    }
    case SC_PutString: {
      DEBUG('a', "PutString exception.\n");
	fprintf(stdout,"R4 %d\n",(char)machine->ReadRegister(2));
      copyStringFromMachine(machine->ReadRegister(4),buff,MAX_STRING_SIZE);fprintf(stdout,"buffer %s\n",buff);
      synchconsole->SynchPutString(buff);// fprintf(stdout,"out of put\n");
      break;
    }
    case SC_Exit: {
      DEBUG('a', "Exit process terminated\n");
      // TODO: check threads ONLY within process address space
      if (currentThread->space->GetNumOfThreads() == 0)
        Exit(machine->ReadRegister(4));
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
    case SC_UserThreadCreate: {
      DEBUG('a', "UserThreadCreate exception.\n");
      do_UserThreadCreate(machine->ReadRegister(4),
                           machine->ReadRegister(5));
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
    default: {
      printf("Unexpected user mode exception %d %d\n", which, type);
      ASSERT(FALSE);
    }

    }
  }
 
  UpdatePC();
}

#endif
