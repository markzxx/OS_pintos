/* Tries to create a file with the null pointer as its name.
   The process must be terminated with exit code -1. */

#include "pintos/src/tests/lib.h"
#include "pintos/src/tests/main.h"

void
test_main (void) 
{
  msg ("create(NULL): %d", create (NULL, 0));
}
