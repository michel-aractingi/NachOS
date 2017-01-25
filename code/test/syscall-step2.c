#include "syscall.h"
void print(char c, int n)
{
  int i;
  for (i = 0; i < n; i++) {
    PutChar(c+i);
  } 
  PutChar('\n');
}
int
main()
{ int num; 
  char string[30];
  PutString("There is a limit on");PutString(" the strings length\n");
  PutString("This string will exceed the Max_String_Size\n");
  PutString("\n.\n.\n.\n");
  PutString("Testing PutChar\n");
  print('a',26);
  PutString("Test Successful\n");
  PutString("\n.\n.\n.\n");
  PutString("Testing PutInt\n");
  PutInt(2301);  PutChar('\t');
  PutInt(-2105);  PutChar('\t');
  PutInt(1909);  PutChar('\t');
  PutInt(1994);  PutChar('\n');
  PutString("Testing GetString\n");
  PutString("Enter String:\t");
  GetString(string);
  PutString(string);
  PutString("Test Successful\n");
  PutString("\n.\n.\n.\n");
  PutString("Testing GetInt\n");
  PutString("Enter integer:\t");
  GetInt(&num);
  PutInt(num);
  PutString("\nTest Successful\n");
  //PutString("\n.\n.\n.\n");
return 0;
}

