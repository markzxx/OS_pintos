/* Tests timer_sleep(-100).  Only requirement is that it not crash. */

#include <stdio.h>
#include "tests.h"
#include "pintos/src/threads/malloc.h"
#include "pintos/src/threads/synch.h"
#include "pintos/src/threads/thread.h"
#include "pintos/src/devices/timer.h"

void
test_alarm_negative (void) 
{
  timer_sleep (-100);
  pass ();
}
