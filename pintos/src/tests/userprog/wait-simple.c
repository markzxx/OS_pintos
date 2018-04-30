/* Wait for a subprocess to finish. */

#include <syscall.h>
#include "pintos/src/tests/lib.h"
#include "pintos/src/tests/main.h"

void
test_main (void) 
{
  msg ("wait(exec()) = %d", wait (exec ("child-simple")));
}
