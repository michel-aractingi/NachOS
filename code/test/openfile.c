#include "syscall.h"
//#include ".h"
void openfiles(void *name){
    //char buffer[N];
    OpenFileId in;
    //int  num_read = 0;

    in = Open((char *) name);
    if(in < 0) {
        PutString("error: file open failed\n");
        Close(in);
        Exit(1);
    }

    //while((num_read = Read(buffer, N, in)) > 0)
      //  Write(buffer, num_read, out);

    PutString("Opened file\n");
    //while(1)
    Close(in);
    UserThreadExit();
    //Close(out);
}

int main() {
    //fprintf(stdout, "ok!!!!!!!!!!!!!!!!!!!!!\n")
    UserThreadCreate(openfiles, (void*)"input");

    UserThreadCreate(openfiles, (void*)"hello");
    UserThreadCreate(openfiles, (void*)"ftest");
    UserThreadCreate(openfiles, (void*)"ftest");
    UserThreadCreate(openfiles, (void*)"ftest");
    UserThreadCreate(openfiles, (void*)"ftest");
    UserThreadCreate(openfiles, (void*)"ftest");
    UserThreadCreate(openfiles, (void*)"ftest");
    UserThreadCreate(openfiles, (void*)"ftest");
    UserThreadCreate(openfiles, (void*)"ftest");
    UserThreadCreate(openfiles, (void*)"ftest");
    UserThreadCreate(openfiles, (void*)"ftest");
    UserThreadCreate(openfiles, (void*)"ftest");
    UserThreadCreate(openfiles, (void*)"ftest");
    UserThreadCreate(openfiles, (void*)"ftest");
    UserThreadCreate(openfiles, (void*)"ftest");
    UserThreadCreate(openfiles, (void*)"ftest");
    UserThreadCreate(openfiles, (void*)"ftest");
    return 0 ;
}
