#include "syscall.h"

int main()
{
  ForkExec("./Th");
  ForkExec("./putstring");
  ForkExec("./putchar");
  ForkExec("./joinTest");
  ForkExec("./userpage1");
  ForkExec("./userpage2");
  return 0;
}
