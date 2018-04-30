/* Accesses a bad address.
   The process must be terminated with -1 exit code. */

#include "pintos/src/tests/lib.h"
#include "pintos/src/tests/main.h"

void
test_main (void)
{
  fail ("bad addr read as %d", *(int *) 0x04000000);
}
