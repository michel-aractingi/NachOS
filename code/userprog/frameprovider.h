#ifndef FRAMEPROVIDER_H
#define FRAMEPROVIDER_H

#include "copyright.h"
#include "bitmap.h"

class FrameProvider {
    public:
        FrameProvider (int n);
        ~FrameProvider ();

        int GetEmptyFrame();
        void ReleaseFrame(int n);
        int NumAvailFrames();
    private:
        BitMap* bitmap;
};
#endif // FRAMEPROVIDER_H
