#include "frameprovider.h"
#include "system.h"

FrameProvider::FrameProvider (int n) {
    this->bitmap = new BitMap(n);
}

FrameProvider::~FrameProvider () {
    delete bitmap;
}

void FrameProvider::ReleaseFrame(int n) {
    this->bitmap->Clear(n);
}
