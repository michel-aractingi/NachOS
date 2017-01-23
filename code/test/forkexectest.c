#include "syscall.h"

int main()

{ // PutString("pass exec\n");
  PutString("T\n");
  ForkExec("user");
  PutString("pass exec 1\n");
  //PutString("pass exec\n");
  ForkExec("put");
  //PutString("pass exec 2\n");
  return 0;
}
