#include "syscall.h"
//#include ".h"

void incr(void *i) {
  *((int *)i)+=1;
  if(*((int *)i) == 4){ PutChar('s');}
  UserThreadExit();
}

void sum(int i, int j) {
  //PutString("Sum of i and j: %d\n", i+j);
  UserThreadExit();
}

int main() {
	//fprintf(stdout, "ok!!!!!!!!!!!!!!!!!!!!!\n");
	PutChar('d');
  UserThreadCreate(incr, (void*)3);
  UserThreadCreate(incr, (void*)5);
  UserThreadCreate(incr, (void*)7);
  return 0 ;
}
