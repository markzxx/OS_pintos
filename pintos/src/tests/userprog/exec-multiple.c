/* Executes and waits for multiple child processes. */

#include <syscall.h>
#include "pintos/src/tests/lib.h"
#include "pintos/src/tests/main.h"

void
test_main (void) 
{
  wait (exec ("child-simple"));
  wait (exec ("child-simple"));
  wait (exec ("child-simple"));
  wait (exec ("child-simple"));
}
