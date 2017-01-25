// addrspace.cc 
//      Routines to manage address spaces (executing user programs).
//
//      In order to run a user program, you must:
//
//      1. link with the -N -T 0 option 
//      2. run coff2noff to convert the object file to Nachos format
//              (Nachos object code format is essentially just a simpler
//              version of the UNIX executable object code format)
//      3. load the NOFF file into the Nachos file system
//              (if you haven't implemented the file system yet, you
//              don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "noff.h"
#include "synch.h"
#include "frameprovider.h"
#include <strings.h>		/* for bzero */
#ifndef USE_TLB
//----------------------------------------------------------------------
// SwapHeader
//      Do little endian to big endian conversion on the bytes in the 
//      object file header, in case the file was generated on a little
//      endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void
SwapHeader (NoffHeader * noffH)
{
    noffH->noffMagic = WordToHost (noffH->noffMagic);
    noffH->code.size = WordToHost (noffH->code.size);
    noffH->code.virtualAddr = WordToHost (noffH->code.virtualAddr);
    noffH->code.inFileAddr = WordToHost (noffH->code.inFileAddr);
    noffH->initData.size = WordToHost (noffH->initData.size);
    noffH->initData.virtualAddr = WordToHost (noffH->initData.virtualAddr);
    noffH->initData.inFileAddr = WordToHost (noffH->initData.inFileAddr);
    noffH->uninitData.size = WordToHost (noffH->uninitData.size);
    noffH->uninitData.virtualAddr =
	WordToHost (noffH->uninitData.virtualAddr);
    noffH->uninitData.inFileAddr = WordToHost (noffH->uninitData.inFileAddr);
}

static Semaphore *threadsLock;   //lock on numberofThreads
static Semaphore *idLock1;       //lock on giveTid
static Semaphore *idLock2;       //lock on idtobitmap
static Semaphore *bit_lock;       //lock on init thread
static void ReadAtVirtual(OpenFile *executable,
                          int virtualaddr,
                          int numBytes,
                          int position,
                          TranslationEntry *pageTable,
                          unsigned numPages) {
  char buff[numBytes];

  TranslationEntry *prevPageTable = machine->pageTable;
  unsigned int prevPageTableSize = machine->pageTableSize;

  executable->ReadAt(buff, numBytes, position);

  machine->pageTable = pageTable;
  machine->pageTableSize = numPages;

  int i;
  for (i = 0; i < numBytes; i++) {
    machine->WriteMem(virtualaddr + i, 1, buff[i]);
  }

  machine->pageTable = prevPageTable;
  machine->pageTableSize = prevPageTableSize;

}

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
//      Create an address space to run a user program.
//      Load the program from a file "executable", and set everything
//      up so that we can start executing user instructions.
//
//      Assumes that the object code file is in NOFF format.
//
//      First, set up the translation from program memory to physical 
//      memory.  For now, this is really simple (1:1), since we are
//      only uniprogramming, and we have a single unsegmented page table
//
//      "executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------
//SemJoin::SemJoin(int n){}
//SemJoin::~SemJoin(){}
AddrSpace::AddrSpace (OpenFile * executable)
{

    NoffHeader noffH;
    unsigned int i, size;

    bit_lock = new Semaphore("Bit Lock Semaphore", 1);
    threadsLock = new Semaphore("lock threads", 1);  
    idLock1 = new Semaphore("lock on Tid",1);   
    idLock2 = new Semaphore("lock on idtobitmap",1); 
    numOfThreads = 0;   
    Tid = 0;
    Pid = machine->givePid();
    executable->ReadAt ((char *) &noffH, sizeof (noffH), 0);
//    fprintf(stdout,"new proc %d\n",Pid);
    if ((noffH.noffMagic != NOFFMAGIC) &&
	(WordToHost (noffH.noffMagic) == NOFFMAGIC))
	SwapHeader (&noffH);
    ASSERT (noffH.noffMagic == NOFFMAGIC);

// how big is address space?
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStackSize;	// we need to increase the size
    // to leave room for the stack
    numPages = divRoundUp (size, PageSize);
    size = numPages * PageSize;

    ASSERT (numPages <= NumPhysPages);	// check we're not trying
    // to run anything too big --
    // at least until we have
    // virtual memory
    
    DEBUG ('a', "Initializing address space, num pages %d, size %d\n",
	   numPages, size);
// first, set up the translation 
    pageTable = new TranslationEntry[numPages];
    for (i = 0; i < numPages; i++)
      {
	  pageTable[i].virtualPage = i;	// for now, virtual page # = phys page #

	  pageTable[i].physicalPage = PFN->GetEmptyFrame() ; 
//fprintf(stdout,"from loop i %d PFN Frame %d \n",i,pageTable[i].physicalPage);
	  pageTable[i].valid = TRUE;
	  pageTable[i].use = FALSE;
	  pageTable[i].dirty = FALSE;
	  pageTable[i].readOnly = FALSE;	// if the code segment was entirely on 
	  // a separate page, we could set its 
	  // pages to be read-only
      }
//fprintf(stdout,"numpages %d\n",numPages);
       
       bitmap  = new BitMap(UserStackSize/NumberOfAreas);
       semJoin = new SemJoin[UserStackSize/NumberOfAreas];
       idToBitmap = new  int [UserStackSize/NumberOfAreas];
       for(i=0;i<UserStackSize/NumberOfAreas;i++){
           idToBitmap[i] = -1;  
}
// zero out the entire address space, to zero the unitialized data segment 
// and the stack segment
      //bzero (machine->mainMemory, size);

// then, copy in the code and data segments into memory
    if (noffH.code.size > 0)
      {
	  DEBUG ('a', "Initializing code segment, at 0x%x, size %d\n",
		 noffH.code.virtualAddr, noffH.code.size);
//      executable->ReadAt (&(machine->mainMemory[noffH.code.virtualAddr]),
  //                       noffH.code.size, noffH.code.inFileAddr);


	   ReadAtVirtual(executable,noffH.code.virtualAddr,noffH.code.size,noffH.code.inFileAddr,pageTable,numPages);


      }
    if (noffH.initData.size > 0)
      {
	  DEBUG ('a', "Initializing data segment, at 0x%x, size %d\n",
		 noffH.initData.virtualAddr, noffH.initData.size);
//      executable->ReadAt (&(machine->mainMemory[noffH.initData.virtualAddr]),
  //                      noffH.initData.size, noffH.initData.inFileAddr);
	   


ReadAtVirtual(executable,noffH.initData.virtualAddr,noffH.initData.size,noffH.initData.inFileAddr,pageTable,numPages);
      }



}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
//      Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace ()
{
  // LB: Missing [] for delete
  // delete pageTable;
 /* if(isChild()){
     this->RemoveChild();
     Child = false;
  }*/
  delete bitmap;
  //delete cond_join;
  //delete lock_join;
 unsigned int i; 
   for (i = 0; i < numPages; i++)
{
     	PFN->ReleaseFrame(i);

}
  delete [] pageTable;
  // End of modification
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
//      Set the initial values for the user-level register set.
//
//      We write these directly into the "machine" registers, so
//      that we can immediately jump to user code.  Note that these
//      will be saved/restored into the currentThread->userRegisters
//      when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters ()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister (PCReg, 0);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, 4);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
    machine->WriteRegister (StackReg, numPages * PageSize - 16);
    DEBUG ('a', "Initializing stack register to %d\n",
	   numPages * PageSize - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
//      On a context switch, save any machine state, specific
//      to this address space, that needs saving.
//
//      For now, nothing!
//----------------------------------------------------------------------

void
AddrSpace::SaveState ()
{
pageTable=machine->pageTable;
numPages = machine->pageTableSize;

}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
//      On a context switch, restore the machine state so that
//      this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void
AddrSpace::RestoreState ()
{
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
}

void AddrSpace::AddThread() {
  threadsLock->P();
  numOfThreads++;
  threadsLock->V();
}

void AddrSpace::ExitThread() {
  threadsLock->P();
  numOfThreads--;
  threadsLock->V();
}

int AddrSpace::GetNumOfThreads() {
  return numOfThreads;
}

bool AddrSpace::isLast(){
  bool stat = false;
  threadsLock->P();
  if(this->GetNumOfThreads()==0) stat = true;
  threadsLock->V();
  return stat;
}
int AddrSpace::giveTid(){
  idLock1->P();
  int a = Tid++;
  idLock1->V();
  return a;
}
int AddrSpace::Findid(int id){
    int i;
    int a = -1;
    idLock2->P();
    for (i = 0 ; i < UserStackSize/NumberOfAreas ; i++){
    if(idToBitmap[i]==id){a =i; break;}
}
    idLock2->V();
    return a;
}
void AddrSpace::Addid(int id,int num){
    idLock2->P();
    idToBitmap[num]=id;
    //fprintf(stdout,"added id %d at %d \n",idToBitmap[num],num);
    idLock2->V();
    
}
void AddrSpace::Removeid(int ThreadNum){
    idLock2->P();
    idToBitmap[ThreadNum] = -1;
    idLock2->V();
}
Thread* AddrSpace::InitUserThread(){
  Thread* newThread = new Thread("user_thread");
  newThread->space = currentThread->space;
  bit_lock->P();
  newThread->numberOfThread = newThread->space->bitmap->Find();
  if((int)newThread->numberOfThread == -1) return NULL;
  newThread->space->semJoin[newThread->numberOfThread].semaphore = new Semaphore ("Join Semaphore",0);
  newThread->space->AddThread();
  newThread->Tid = newThread->space->giveTid();
  newThread->space->Addid(newThread->Tid,newThread->numberOfThread);
  bit_lock->V();
  return newThread;

}
int AddrSpace::GetPid(){
return Pid;
}
#endif