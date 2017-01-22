// progtest.cc 
//      Test routines for demonstrating that Nachos can load
//      a user program and execute it.  
//
//      Also, routines for testing the Console hardware device.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "console.h"
#include "addrspace.h"
#include "synch.h"
#include "synchconsole.h"
#include "filetable.h"
//----------------------------------------------------------------------
// StartProcess
//      Run a user program.  Open the executable, load it into
//      memory, and jump to it.
//----------------------------------------------------------------------
class FileVector;

void
StartProcess (char *filename)
{
    OpenFile *executable = fileSystem->Open (filename,1);
    AddrSpace *space;

    if (executable == NULL)
      {
	  printf ("Unable to open files %s\n", filename);
	  return;
      }
    space = new AddrSpace (executable);
    currentThread->space = space;


//CHANGED TO ADD ID TO INITIAL THREAD
   currentThread->Tid = space->giveTid();
   currentThread->numberOfThread = space->bitmap->Find();
   space->Addid(currentThread->Tid,currentThread->numberOfThread);
   currentThread->space->semJoin[currentThread->numberOfThread].semaphore = new Semaphore ("Join Semaphore",0);

    FileVector *vector = new(std::nothrow) FileVector();
    space->fileVector = vector;

///
    delete executable;		// close file

    space->InitRegisters ();	// set the initial register values
    space->RestoreState ();	// load page table register

    machine->Run ();		// jump to the user progam
    ASSERT (FALSE);		// machine->Run never returns;
    // the address space exits
    // by doing the syscall "exit"
}

// Data structures needed for the console test.  Threads making
// I/O requests wait on a Semaphore to delay until the I/O completes.

static Console *console;
static Semaphore *readAvail;
static Semaphore *writeDone;

//----------------------------------------------------------------------
// ConsoleInterruptHandlers
//      Wake up the thread that requested the I/O.
//----------------------------------------------------------------------

static void
ReadAvail (int arg)
{
    readAvail->V ();
}
static void
WriteDone (int arg)
{
    writeDone->V ();
}

//----------------------------------------------------------------------
// ConsoleTest
//      Test the console by echoing characters typed at the input onto
//      the output.  Stop when the user types a 'q'.
//----------------------------------------------------------------------

void
ConsoleTest (char *in, char *out)
{
    char ch;
    
    console = new Console (in, out, ReadAvail, WriteDone, 0);
    readAvail = new Semaphore ("read avail", 0);
    writeDone = new Semaphore ("write done", 0);

    int begin = 1;       //BEGIN flag to indicate a beginning of a line
    for (;;)
      {
	  //writeDone->P();
  
	  readAvail->P ();	// wait for character to arrive
	  
	  ch = console->GetChar ();
	  if ( ch == EOF ) return;
		
          if (begin) {
            console->PutChar('<');
            begin = 0;
	    writeDone->P();
          }
	 
          if (ch == '\n' ){
		 console->PutChar('>');
		 writeDone->P();
		 console->PutChar('\n');	
	       	 begin = 1; 
          }
	
          
	  else	  console->PutChar (ch);        // echo it! 
          
	  writeDone->P ();      // wait for write to finish

	  if (ch == 'q')
	      return;		// if q, quit
      }
}
void   SynchConsoleTest (char *in, char *out)
{
	char ch;
//	char ch[4];
	//SynchConsole *synchconsole = new SynchConsole(in, out);
	while ((ch = synchconsole->SynchGetChar()) != EOF)
		synchconsole->SynchPutChar(ch);

	/*synchconsole->SynchGetString(synchconsole ,ch ,4);
	synchconsole->SynchPutString(synchconsole ,ch);*/
fprintf(stderr, "Solaris: EOF detected in SynchConsole!\n");
}
