/* Child process of mmap-exit.
   Mmaps a file and writes to it via the mmap'ing, then exits
   without calling munmap.  The data in the mapped region must be
   written out at program termination. */

#include <string.h>
#include <syscall.h>
#include "sample.inc"
#include "pintos/src/tests/lib.h"
#include "pintos/src/tests/main.h"

#define ACTUAL ((void *) 0x10000000)

void
test_main (void)
{
  int handle;

  CHECK (create ("sample.txt", sizeof sample), "create \"sample.txt\"");
  CHECK ((handle = open ("sample.txt")) > 1, "open \"sample.txt\"");
  CHECK (mmap (handle, ACTUAL) != MAP_FAILED, "mmap \"sample.txt\"");
  memcpy (ACTUAL, sample, sizeof sample);
}

