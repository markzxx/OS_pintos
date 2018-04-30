/* Expand the stack by 32 bytes all at once using the PUSHA
   instruction.
   This must succeed. */

#include <string.h>
#include "pintos/src/tests/arc4.h"
#include "pintos/src/tests/cksum.h"
#include "pintos/src/tests/lib.h"
#include "pintos/src/tests/main.h"

void
test_main (void)
{
  asm volatile
    ("movl %%esp, %%eax;"        /* Save a copy of the stack pointer. */
     "andl $0xfffff000, %%esp;"  /* Move stack pointer to bottom of page. */
     "pushal;"                   /* Push 32 bytes on stack at once. */
     "movl %%eax, %%esp"         /* Restore copied stack pointer. */
     : : : "eax");               /* Tell GCC we destroyed eax. */
}
