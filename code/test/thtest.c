#include "syscall.h"

void print( void *a){

PutChar('a');PutChar('\n');
UserThreadExit();
}

int main(){


UserThreadCreate(print,(void*)1);
UserThreadCreate(print,(void*)1);
UserThreadCreate(print,(void*)1);
UserThreadCreate(print,(void*)1);
UserThreadCreate(print,(void*)1);





}
