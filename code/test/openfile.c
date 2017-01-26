#include "syscall.h"

void fileopener(void* id){
    OpenFileId id0 = Open("main/small0");
    OpenFileId id1 = Open("main/submain/input0");
    char buffer[100];
    int num_read = 0;
    while((num_read = Read(buffer, 100, id0)) > 0)
        Write(buffer, num_read, id1);

    OpenFileId id2 = Open("main/submain/input1");
    OpenFileId id3 = Open("main/submain/input2");
    OpenFileId id4 = Open("main/submain/input3");
    OpenFileId id5 = Open("main/submain/input4");
    OpenFileId id6 = Open("main/submain/input5");
    OpenFileId id7 = Open("main/submain/input6");
    OpenFileId id8 = Open("main/small");
    Close(id0);
    Close(id1);
    Close(id2);
    Close(id3);
    Close(id4);
    Close(id5);
    Close(id6);
    Close(id7);
    Close(id8);
    UserThreadExit();
}

void threadhandler(void* id){
    OpenFileId id0 = Open("main/small0");
    OpenFileId id1 = Open("main/submain/input0");
    char buffer[100];
    int num_read = 0;
    while((num_read = Read(buffer, 100, id1)) > 0)
        Write(buffer, num_read, id0);
    OpenFileId id2 = Open("main/submain/input1");
    OpenFileId id3 = Open("main/submain/input2");
    OpenFileId id4 = Open("main/submain/input3");
    OpenFileId id5 = Open("main/submain/input4");
    OpenFileId id6 = Open("main/submain/input5");
    OpenFileId id7 = Open("main/submain/input6");
    OpenFileId id8 = Open("main/submain/input6");
    OpenFileId id9 = Open("main/small0");
    OpenFileId id10 = Open("main/small1");
    Close(id0);
    Close(id1);
    Close(id2);
    Close(id3);
    Close(id4);
    OpenFileId id11 = Open("main/small2");
    OpenFileId id12 = Open("main/small3");
    Close(id5);
    Close(id6);
    Close(id7);
    Close(id8);
    Close(id9);
    Close(id10);
    Close(id11);
    Close(id12);
    UserThreadExit();
}

int main() {
    UserThreadCreate(threadhandler, (void*)"input");
    //UserThreadCreate(fileopener, (void*) "1" );
    return 0 ;
}
