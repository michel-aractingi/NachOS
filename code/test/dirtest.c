//
// Created by linux on 1/22/17.
//

#include "syscall.h"

#define N 1024

void
createdir(char *name) {
    int result;
    result = MakeDirectory(name);
    if(result == -1) {
        PutString("error: could not create Directory\n");
        Exit(1);
    }
    PutString("created directory: ");
    PutString(name);
    PutString("\n");
}

void
changedir(char *name) {
    int result;
    result = ChangeDirectory(name);
    if(result == -1) {
        PutString("error: ChangeDir() failed\n");
        Exit(1);
    }

    PutString("changed directory\n");
}

OpenFileId
create_and_open_file(char *name) {
    OpenFileId id;
    Create(name);
    id = Open(name);
    if(id < 0) {
        PutString("error: file open failed\n");
        Close(id);
        Exit(1);
    }

    PutString("created file: ");
    PutString(name);
    PutString("\n");
    return id;
}


int main() {
    char buffer[N];
    OpenFileId in, out;
    int  num_read = 0;

    in = Open("input");
    if(in < 0) {
        PutString("error: file open failed\n");
        Close(in);
        Exit(1);
    }

    createdir("hello");
    createdir("hello/world");
    createdir("hello/world/foo");
    changedir("hello/world/foo");
    out = create_and_open_file("bar");

    while((num_read = Read(buffer, N, in)) > 0)
        Write(buffer, num_read, out);

    PutString("copied over file\n");
    Close(in);
    Close(out);
    Exit(0);
}