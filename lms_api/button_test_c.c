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
#include "ev3_button.h"

#define TITLE_DELAY 1000
#define DELAY       2000

void buttons()
{
  unsigned short but;

  printf("Buttons\n");
  while (true)
  {
    but = ButtonWaitForAnyPress(TITLE_DELAY);
    if (but == 0)
        printf("None");
    if ((but & BUTTON_ID_ENTER) != 0)
        printf("Enter ");
    if ((but & BUTTON_ID_LEFT) != 0)
        printf("Left ");
    if ((but & BUTTON_ID_RIGHT) != 0)
        printf("Right ");
    if ((but & BUTTON_ID_UP) != 0)
        printf("Up ");
    if ((but & BUTTON_ID_DOWN) != 0)
        printf("Down ");
    if ((but & BUTTON_ID_ESCAPE) != 0)
        printf("Escape ");
    printf("\n");
    if (but == BUTTON_ID_ESCAPE)
        break;
  }
  ButtonWaitForAnyPress(TITLE_DELAY);
}

void leds()
{
  int i;
  printf("LEDS\n");
  ButtonWaitForAnyPress(TITLE_DELAY);
  for(i = LED_BLACK; i < NUM_LED_PATTERNS; i++)
  {
    printf("Pattern %d\n", i);
    SetLedPattern(i);
    Wait(SEC_1);
    SetLedWarning(TRUE);
    Wait(SEC_1);
    SetLedWarning(FALSE);
    ButtonWaitForAnyPress(DELAY*2);
  }
  SetLedPattern(LED_BLACK);
  ButtonWaitForAnyPress(DELAY*2);
}



int main()
{
  ButtonLedInit();
  buttons();
  leds();
  printf("hardware version = %f\n", HardwareVersion());
  printf("hardware version string = %s\n", HardwareVersionString());
  ButtonLedExit();
}

