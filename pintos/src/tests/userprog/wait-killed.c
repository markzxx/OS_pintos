/* Wait for a process that will be killed for bad behavior. */

#include <syscall.h>
#include "pintos/src/tests/lib.h"
#include "pintos/src/tests/main.h"

void
test_main (void) 
{
  msg ("wait(exec()) = %d", wait (exec ("child-bad")));
}
