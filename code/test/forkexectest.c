#include "syscall.h"

int main()
{
  PutString("He");
  ForkExec("./userpages0");
    PutString("Here");
  //ForkExec("./userpages2");
  //for(int i=0;i<10000000;i++);
  return 0;
}