#include "syscall.h"

int main()
{ // PutString("pass exec\n");
  ForkExec("./putstring");
  //PutString("pass exec 1\n");
  //PutString("pass exec\n");
  ForkExec("./putchar");
  ForkExec("./putstring");
    ForkExec("./putchar");
    ForkExec("./putchar");
  return 0;
}
