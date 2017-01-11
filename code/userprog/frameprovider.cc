#include "frameprovider.h"
#include "system.h"

FrameProvider::FrameProvider (int n) {
    bitmap = new BitMap(n);
}

FrameProvider::~FrameProvider () {
    delete bitmap;
}

void FrameProvider::ReleaseFrame(int n) {
    bitmap->Clear(n);
}

int FrameProvider::GetEmptyFrame(int n) {
    int f = bitmap->Find();
    bzero(&machine->mainMemory[f], PageSize);
    return f;
}

int FrameProvider::NumAvailFrames() {
    return bitmap->NumClear();
}
