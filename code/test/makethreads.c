#include "syscall.h"
//#include ".h"

void incr(void* i) {
  PutString("\nincr\n");
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
  //UserThreadCreate(incr, (void*)5);
  return 0 ;
}
