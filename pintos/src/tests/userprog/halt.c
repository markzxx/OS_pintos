/* Tests the halt system call. */

#include "pintos/src/tests/lib.h"
#include "pintos/src/tests/main.h"

void
test_main (void) 
{
  halt ();
  fail ("should have halted");
}
