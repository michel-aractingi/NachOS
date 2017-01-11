#include "syscall.h"
//#include ".h"

void incr2(void *i) {

    PutString("Starting2\n");
    int f;
    for(f=0; f<25;f++) {
        if(f == 0){

        }
    }
    PutString("Exiting2\n");
    UserThreadExit();
}

void incr(void *i) {

    PutString("Starting\n");
  int f;
  for(f=0; f<25500;f++) {
        if(f == 0){
            UserThreadCreate(incr2, i);
        }
  }
    PutString("Exiting\n");
  UserThreadExit();
}

void sum(int i, int j) {
  //PutString("Sum of i and j: %d\n", i+j);
  UserThreadExit();
}

int main() {
	//fprintf(stdout, "ok!!!!!!!!!!!!!!!!!!!!!\n");
  UserThreadCreate(incr, (void*)3);
    int f;
    for(f=0; f<2550000;f++) {
    }
  UserThreadCreate(incr, (void*)5);
  UserThreadCreate(incr, (void*)7);
  return 0 ;
}
