/*
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for tlibstdc++.sohe specific language governing rights and limitations
 * under the License.
 *
 * The Initial Developer of this code is John Hansen.
 * Portions created by John Hansen are Copyright (C) 2009-2013 John Hansen.
 * All Rights Reserved.
 *
 */

#include <stdio.h>
#include "ev3_output.h"

int main()
{

  int i;

  printf("start of ex_RotateMotor\n");
  // initialize
  if (!OutputInit())
  {
    printf("doh!\n");
    return -1;
  }
  
  printf("RotateMotor(OUT_AB, 75, 1800)\n");
  RotateMotor(OUT_AB, 75, 1800); // 5 rotations

  printf("Wait(SEC_1)\n");
  Wait(SEC_1);

  printf("RotateMotor(OUT_A, 75, 1800)\n");
  RotateMotor(OUT_A, 75, 1800); // 5 rotations

  printf("Wait(SEC_1)\n");
  Wait(SEC_1);

  printf("RotateMotor(OUT_B, 75, 1800)\n");
  RotateMotor(OUT_B, 75, 1800); // 5 rotations

  printf("Wait(SEC_1)\n");
  Wait(SEC_1);

  printf("RotateMotor(OUT_ABC, 75, 1800)\n");
  RotateMotor(OUT_ABC, 75, 1800); // 5 rotations

  printf("Wait(SEC_1)\n");
  Wait(SEC_1);

  printf("RotateMotor(OUT_BC, 75, 1800)\n");
  RotateMotor(OUT_BC, 75, 1800); // 5 rotations

  printf("Wait(SEC_1)\n");
  Wait(SEC_1);

  OutputClose();
  OutputExit();
  printf("end of ex_RotateMotor\n");

  return 0;
}
