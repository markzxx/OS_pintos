/* Reads a 128 kB file onto the stack and "sorts" the bytes in
   it, using quick sort, a multi-pass divide and conquer
   algorithm.  The sorted data is written back to the same file
   in-place. */

#include <debug.h>
#include <syscall.h>
#include "pintos/src/tests/lib.h"
#include "pintos/src/tests/main.h"
#include "qsort.h"

const char *test_name = "child-qsort";

int
main (int argc UNUSED, char *argv[]) 
{
  int handle;
  unsigned char buf[128 * 1024];
  size_t size;

  quiet = true;

  CHECK ((handle = open (argv[1])) > 1, "open \"%s\"", argv[1]);

  size = read (handle, buf, sizeof buf);
  qsort_bytes (buf, sizeof buf);
  seek (handle, 0);
  write (handle, buf, size);
  close (handle);
  
  return 72;
}
