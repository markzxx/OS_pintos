/* Maps and unmaps a file and verifies that the mapped region is
   inaccessible afterward. */

#include <syscall.h>
#include "sample.inc"
#include "pintos/src/tests/lib.h"
#include "pintos/src/tests/main.h"

#define ACTUAL ((void *) 0x10000000)

void
test_main (void)
{
  int handle;
  mapid_t map;

  CHECK ((handle = open ("sample.txt")) > 1, "open \"sample.txt\"");
  CHECK ((map = mmap (handle, ACTUAL)) != MAP_FAILED, "mmap \"sample.txt\"");

  munmap (map);

  fail ("unmapped memory is readable (%d)", *(int *) ACTUAL);
}
