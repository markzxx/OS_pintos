/* Verifies that memory mappings persist after file close. */

#include <string.h>
#include <syscall.h>
#include "sample.inc"
#include "pintos/src/tests/arc4.h"
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

  close (handle);

  if (memcmp (ACTUAL, sample, strlen (sample)))
    fail ("read of mmap'd file reported bad data");

  munmap (map);
}
