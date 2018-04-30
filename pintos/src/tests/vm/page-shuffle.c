/* Shuffles a 128 kB data buffer 10 times, printing the checksum
   after each time. */

#include <stdbool.h>
#include "pintos/src/tests/arc4.h"
#include "pintos/src/tests/cksum.h"
#include "pintos/src/tests/lib.h"
#include "pintos/src/tests/main.h"

#define SIZE (128 * 1024)

static char buf[SIZE];

void
test_main (void)
{
  size_t i;

  /* Initialize. */
  for (i = 0; i < sizeof buf; i++)
    buf[i] = i * 257;
  msg ("init: cksum=%lu", cksum (buf, sizeof buf));
    
  /* Shuffle repeatedly. */
  for (i = 0; i < 10; i++)
    {
      shuffle (buf, sizeof buf, 1);
      msg ("shuffle %zu: cksum=%lu", i, cksum (buf, sizeof buf));
    }
}
