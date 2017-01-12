#include "syscall.h"

int main()
{
  ForkExec("../test/userpages1");
  ForkExec("../test/userpages2");
  return 0;
}