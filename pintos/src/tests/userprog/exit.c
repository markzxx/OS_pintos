/* Tests the exit system call. */

#include "pintos/src/tests/lib.h"
#include "pintos/src/tests/main.h"

void
test_main (void) 
{
  exit (57);
  fail ("should have called exit(57)");
}
