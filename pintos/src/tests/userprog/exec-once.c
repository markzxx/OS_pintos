/* Executes and waits for a single child process. */

#include <syscall.h>
#include "pintos/src/tests/lib.h"
#include "pintos/src/tests/main.h"

void
test_main (void) 
{
  wait (exec ("child-simple"));
}
