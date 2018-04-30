/* Demonstrate that the stack can grow.
   This must succeed. */

#include <string.h>
#include "pintos/src/tests/arc4.h"
#include "pintos/src/tests/cksum.h"
#include "pintos/src/tests/lib.h"
#include "pintos/src/tests/main.h"

void
test_main (void)
{
  char stack_obj[4096];
  struct arc4 arc4;

  arc4_init (&arc4, "foobar", 6);
  memset (stack_obj, 0, sizeof stack_obj);
  arc4_crypt (&arc4, stack_obj, sizeof stack_obj);
  msg ("cksum: %lu", cksum (stack_obj, sizeof stack_obj));
}
