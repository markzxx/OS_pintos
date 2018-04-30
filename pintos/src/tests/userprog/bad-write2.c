/* This program attempts to write to kernel memory. 
   This should terminate the process with a -1 exit code. */

#include "pintos/src/tests/lib.h"
#include "pintos/src/tests/main.h"

void
test_main (void) 
{
  *(int *)0xC0000000 = 42;
  fail ("should have exited with -1");
}