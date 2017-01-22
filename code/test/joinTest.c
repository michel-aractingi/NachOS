#include "syscall.h"
//#include ".h"

void incr(void *i) {
 /*((int *)i)+=1;
  //int j;
 // for ( j=0;j<10000;j++){}
  if(*((int *)i) == 4){ 
	PutChar('s');	
	PutChar('\n');}*/
  int f;
    for(f=0; f<25500;f++) {}
  PutChar('s');PutChar('\n');
  UserThreadExit();
}

void sum(void *i) {
 //inst thread_num = *((int *)i) ;
 //   PutString("argument for join");PutInt(*((int *)i));PutChar('\n');
  UserThreadJoin( *((int *)i));
  UserThreadExit();
}

int main() {
	//fprintf(stdout, "ok!!!!!!!!!!!!!!!!!!!!!\n");
	//	PutChar('1');PutChar('\n');
    	int c =UserThreadCreate(incr, (void*)3);
	//UserThreadCreate(sum, (void*)0);
        //	PutChar('2');PutChar('\n');
        int a  = UserThreadCreate(incr, (void*)3);
        UserThreadJoin(a);        
 	//	PutChar('3');PutChar('\n');
    	//UserThreadCreate(sum, (void*)1);
        //UserThreadCreate(sum, (void*)2);
	int b = UserThreadCreate(sum, (void*)c);
    	UserThreadCreate(sum, (void*)b);     
 //       PutString("ab\n");PutInt(a-1);PutInt(b-1);
	UserThreadCreate(incr, (void*)b);
     	/*UserThreadCreate(incr, (void*)3);     
	UserThreadCreate(incr, (void*)3);     
	UserThreadCreate(incr, (void*)3);*/
     //   PutInt(a);
   //     PutChar('\n');
 //       PutInt(b);

 return 0 ;
}
