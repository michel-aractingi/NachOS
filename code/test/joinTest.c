#include "syscall.h"
//#include ".h"

void incr(void *i) {
 /*((int *)i)+=1;
  //int j;
 // for ( j=0;j<10000;j++){}
  if(*((int *)i) == 4){ 
	PutChar('s');	
	PutChar('\n');}*/
  PutChar('s');PutChar('\n');
  UserThreadExit();
}

void sum(void *i) {
 //int thread_num = *((int *)i) ;
  UserThreadJoin( *((int *)i));
}

int main() {
	//fprintf(stdout, "ok!!!!!!!!!!!!!!!!!!!!!\n");
	//	PutChar('1');PutChar('\n');
    	UserThreadCreate(incr, (void*)3);
	//UserThreadCreate(sum, (void*)0);
        //	PutChar('2');PutChar('\n');
        UserThreadCreate(incr, (void*)3);
 	//	PutChar('3');PutChar('\n');
    	UserThreadCreate(sum, (void*)1);
        UserThreadCreate(sum, (void*)2);
	//UserThreadCreate(incr, (void*)3);
    	//UserThreadCreate(incr, (void*)5);

 return 0 ;
}
