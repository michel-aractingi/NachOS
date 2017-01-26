#include "syscall.h"

void delay(void *a){
 int i;
 for (i = 0 ; i< 20000 ; i++){}
 UserThreadExit();
}
int main (){
PutString("added Threads with IDs : \n");
int a  = UserThreadCreate(delay,(void*)1);
int b = UserThreadCreate(delay,(void*)2);
PutInt(a);PutChar('\n');PutInt(b);PutChar('\n');

 a  = UserThreadCreate(delay,(void*)1);
 b = UserThreadCreate(delay,(void*)2);
PutInt(a);PutChar('\n');PutInt(b);PutChar('\n');

 a  = UserThreadCreate(delay,(void*)1);
 b = UserThreadCreate(delay,(void*)2);
PutInt(a);PutChar('\n');PutInt(b);PutChar('\n');

 a  = UserThreadCreate(delay,(void*)1);
 b = UserThreadCreate(delay,(void*)2);
PutInt(a);PutChar('\n');PutInt(b);PutChar('\n');

 a  = UserThreadCreate(delay,(void*)1);
 b = UserThreadCreate(delay,(void*)2);
PutInt(a);PutChar('\n');PutChar('\n');PutInt(b);PutChar('\n');

return 0;


}
