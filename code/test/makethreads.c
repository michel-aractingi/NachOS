#include "syscall.h"
//#include ".h"

void incr(void* i) {
	PutChar('s');
  PutString("Next: %d");
  PutString((char*)(i));
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
  return 0 ;
}
