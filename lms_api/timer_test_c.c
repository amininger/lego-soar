/*
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * The Initial Developer of this code is John Hansen.
 * Portions created by John Hansen are Copyright (C) 2009-2013 John Hansen.
 * All Rights Reserved.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "ev3_timer.h"
#include "ev3_command.h"

void Handler(int sig)
{
  printf("handler %d\n", sig);
}

int main ()
{
  int i;
  int ms;
  unsigned long t, ft, mt;
  
  TimerInit();

  SetTimerCallback(ti1sec, &Handler);
  
  /* initialize random seed: */
  srand(time(NULL));

  ClearTimer(0);
  ClearTimerMS(0);
  for(i = 0; i < 10; i++)
  {
    /* generate random number between 1 and 1000: */
    ms = rand() % 1000 + 1;
    printf("Wait(%d)\n", ms);
    Wait(ms);
    t = Timer(0);
    ft = FastTimer(0);
    mt = TimerMS(0);
    printf("Timer(0) = %d\n", t);
    printf("FastTimer(0) = %d\n", ft);
    printf("TimerMS(0) = %d\n", mt);
//    usleep(1000000);
  }
}

