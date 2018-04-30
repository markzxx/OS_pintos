/* Checks that growing a file updates the file position
   correctly. */

#include <syscall.h>
#include "pintos/src/tests/filesys/seq-test.h"
#include "pintos/src/tests/lib.h"
#include "pintos/src/tests/main.h"

static char buf[2134];

static size_t
return_block_size (void) 
{
  return 37;
}

static void
check_tell (int fd, long ofs) 
{
  long pos = tell (fd);
  if (pos != ofs)
    fail ("file position not updated properly: should be %ld, actually %ld",
          ofs, pos);
}

void
test_main (void) 
{
  seq_test ("foobar",
            buf, sizeof buf, 0,
            return_block_size, check_tell);
}
