#include "syscall.h"
//#include ".h"
void delay(void *a){
 int i;
 for (i = 0 ; i< 20000 ; i++){}
}
void incr(void *i) {
  delay((void*)1);
  UserThreadExit();
}

void sum(void *i) {
  UserThreadJoin( *((int *)i));
  PutString("Join on ");PutInt(*((int *)i));PutString(" Successful\n");
  UserThreadExit();
}

int main() {

    	int c =UserThreadCreate(incr, (void*)3);
	int a  = UserThreadCreate(incr, (void*)3);
        UserThreadJoin(a);PutString("Join on ");PutInt(a);PutString(" Successful\n");   
        int b = UserThreadCreate(sum, (void*)c);
    	UserThreadCreate(sum, (void*)b);     
	//UserThreadCreate(sum, (void*));

 return 0 ;
}
