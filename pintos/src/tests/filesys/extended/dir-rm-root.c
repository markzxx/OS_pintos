/* Try to remove the root directory.
   This must fail. */

#include <syscall.h>
#include "pintos/src/tests/lib.h"
#include "pintos/src/tests/main.h"

void
test_main (void) 
{
  CHECK (!remove ("/"), "remove \"/\" (must fail)");
  CHECK (create ("/a", 243), "create \"/a\"");
}
