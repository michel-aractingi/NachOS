#include "console.h"

#ifdef CHANGED

#ifndef SYNCHCONSOLE_H
#define SYNCHCONSOLE_H

#include "copyright.h"
#include "utility.h"
#include "console.h"

class SynchConsole {
public:
  SynchConsole(char *readFile, char *writeFile);
  // initialize the hardware console device
  ~SynchConsole();                    // clean up console emulation
  void SynchPutChar(const char ch);   // Unix putchar(3S)
  char SynchGetChar();                // Unix getchar(3S)
  void SynchPutString(/*SynchConsole *self, */const char *s); // Unix puts(3S)
  void SynchGetString(/*SynchConsole *self ,*/char *s, int n);       // Unix fgets(3S)
private:
  Console *console;
};

#endif // SYNCHCONSOLE_H

#endif // CHANGED
