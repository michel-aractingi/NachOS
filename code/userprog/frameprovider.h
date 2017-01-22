#ifndef FRAMEPROVIDER_H
#define FRAMEPROVIDER_H

#include "copyright.h"
#include "bitmap.h"
#include "synch.h"
class FrameProvider {
    public:
        FrameProvider (int n);
        ~FrameProvider ();

        int GetEmptyFrame();
        void ReleaseFrame(int n);
        int NumAvailFrames();
    private:
        BitMap* bitmap;
        Semaphore *frameSem;
};
#endif // FRAMEPROVIDER_H
