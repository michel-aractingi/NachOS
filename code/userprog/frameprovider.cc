#include "frameprovider.h"
#include "system.h"
#include "synch.h"
FrameProvider::FrameProvider (int n) {
    bitmap = new BitMap(n);
    frameSem = new Semaphore ("lock on frame find",1);
}

FrameProvider::~FrameProvider () {
    delete bitmap;
}

void FrameProvider::ReleaseFrame(int n) {
    frameSem->P();
    bitmap->Clear(n);
    frameSem->V();
}

int FrameProvider::GetEmptyFrame() {
  //  Semaphore *frameSem = new Semaphore ("lock on frame find",1);
    frameSem->P();
    int f = bitmap->Find();
    frameSem->V();
    bzero(&(machine->mainMemory[f*PageSize]), PageSize);
    return f;
}

int FrameProvider::NumAvailFrames() {
    frameSem->P();
    int N = bitmap->NumClear();
    frameSem->V();
    return N;
}
