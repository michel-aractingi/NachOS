#ifndef FRAMEPROVIDER_H
#define FRAMEPROVIDER_H

#include "copyright.h"
#include "bitmap.h"

class FrameProvider {
    public:
        FrameProvider (int);
        ~FrameProvider ();

        int GetEmptyFrame(int);
        void ReleaseFrame(int);
        int NumAvailFrames();

    private:
        BitMap* bitmap;
};
#endif // FRAMEPROVIDER_H
