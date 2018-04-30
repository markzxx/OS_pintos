/* Wait for a subprocess to finish, twice.
   The first call must wait in the usual way and return the exit code.
   The second wait call must return -1 immediately. */

#include <syscall.h>
#include "pintos/src/tests/lib.h"
#include "pintos/src/tests/main.h"

void
test_main (void) 
{
  pid_t child = exec ("child-simple");
  msg ("wait(exec()) = %d", wait (child));
  msg ("wait(exec()) = %d", wait (child));
}
