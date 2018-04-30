/* Tests timer_sleep(0), which should return immediately. */

#include <stdio.h>
#include "tests.h"
#include "pintos/src/threads/malloc.h"
#include "pintos/src/threads/synch.h"
#include "pintos/src/threads/thread.h"
#include "pintos/src/devices/timer.h"

void
test_alarm_zero (void) 
{
  timer_sleep (0);
  pass ();
}
