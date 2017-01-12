#include "syscall.h"

void a(void *i) {
    int f;
    for(f=0; f<5;f++) {
      PutChar('a');
    }
    UserThreadExit();
}

void b(void *i) {
    int g;
    for(g=0; g<10;g++) {
      PutChar('b');
    }
    UserThreadExit();
}

int main() {
  UserThreadCreate(a, (void*)5);
  UserThreadCreate(b, (void*)7);
  return 0 ;
}
