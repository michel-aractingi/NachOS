#include "frameprovider.h"
#include "system.h"
#include "synch.h"

FrameProvider::FrameProvider (int n) {
    bitmap = new BitMap(n);
}

FrameProvider::~FrameProvider () {
    delete bitmap;
}

void FrameProvider::ReleaseFrame(int n) {
    bitmap->Clear(n);
}

int FrameProvider::GetEmptyFrame() {
    Semaphore *frameSem = new Semaphore ("lock on frame find",1);
    frameSem->P();
    int f = bitmap->Find();
    frameSem->V();
    bzero(&machine->mainMemory[f], PageSize);
    return f;
}

int FrameProvider::NumAvailFrames() {
    return bitmap->NumClear();
}
