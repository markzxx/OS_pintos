#include "pintos/src/tests/main.h"
#include "parallel-merge.h"

void
test_main (void) 
{
  parallel_merge ("child-qsort-mm", 80);
}
