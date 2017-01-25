#include "syscall.h"

int main()

{ // PutString("pass exec\n");
  PutString("T\n");
  ForkExec("userpage1");
  PutString("pass exec 1\n");
  //PutString("pass exec\n");
  ForkExec("putstring");
  //PutString("pass exec 2\n");
  return 0;
}
