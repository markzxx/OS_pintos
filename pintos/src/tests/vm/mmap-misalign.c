/* Verifies that misaligned memory mappings are disallowed. */

#include <syscall.h>
#include "pintos/src/tests/lib.h"
#include "pintos/src/tests/main.h"

void
test_main (void) 
{
  int handle;
  
  CHECK ((handle = open ("sample.txt")) > 1, "open \"sample.txt\"");
  CHECK (mmap (handle, (void *) 0x10001234) == MAP_FAILED,
         "try to mmap at misaligned address");
}

