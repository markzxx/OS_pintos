/* Read from an address 4,096 bytes below the stack pointer.
   The process must be terminated with -1 exit code. */

#include <string.h>
#include "pintos/src/tests/arc4.h"
#include "pintos/src/tests/cksum.h"
#include "pintos/src/tests/lib.h"
#include "pintos/src/tests/main.h"

void
test_main (void)
{
  asm volatile ("movl -4096(%esp), %eax");
}
