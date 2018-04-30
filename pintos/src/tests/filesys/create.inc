/* -*- c -*- */

#include <syscall.h>
#include "pintos/src/tests/lib.h"
#include "pintos/src/tests/main.h"

static char buf[TEST_SIZE];

void
test_main (void) 
{
  const char *file_name = "blargle";
  CHECK (create (file_name, TEST_SIZE), "create \"%s\"", file_name);
  check_file (file_name, buf, TEST_SIZE);
}
