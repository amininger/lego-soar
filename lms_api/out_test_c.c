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
#include "ev3_output.h"

int main()
{

  int i;

  printf("hello world\n");

  printf("start of out_test\n");
  Wait(SEC_1);

  // initialize
  if (!OutputInit())
    printf("output init returned false\n");

  ResetAllTachoCounts(OUT_ABCD);

//  OutputSetType(OUT_A, DEVICE_TYPE_TACHO);
//  OutputSetType(OUT_B, DEVICE_TYPE_TACHO);
//  OutputSetType(OUT_C, DEVICE_TYPE_MINITACHO);
//  OutputSetTypes(DEVICE_TYPE_TACHO, DEVICE_TYPE_TACHO, DEVICE_TYPE_TACHO, DEVICE_TYPE_TACHO);

  SetPower(OUT_A, 90);
  SetSpeed(OUT_B, 40);
  SetPower(OUT_C, 60);
  SetPower(OUT_D, -60);
  On(OUT_ALL);

  bool isBusy = false;
  for(i=0; i < 10; i++)
  {
    printf("OUT_A: %d %d %d\n", MotorRotationCount(OUT_A), MotorTachoCount(OUT_A), MotorActualSpeed(OUT_A));
    printf("OUT_B: %d %d %d\n", MotorRotationCount(OUT_B), MotorTachoCount(OUT_B), MotorActualSpeed(OUT_B));
    printf("OUT_C: %d %d %d\n", MotorRotationCount(OUT_C), MotorTachoCount(OUT_C), MotorActualSpeed(OUT_C));
    printf("OUT_D: %d %d %d\n", MotorRotationCount(OUT_D), MotorTachoCount(OUT_D), MotorActualSpeed(OUT_D));
    Wait(SEC_1);

    isBusy = MotorBusy(OUT_ALL);
    printf("Any output isBusy = %d\n", (byte)isBusy);
  }
//  Wait(SEC_5);

  printf("Float(OUT_ALL)\n");
  Float(OUT_ALL);

  printf("Wait(SEC_2)\n");
  Wait(SEC_2);

  printf("ResetAllTachoCounts(OUT_ALL)\n");
  ResetAllTachoCounts(OUT_ALL);

  printf("OUT_A: %d %d %d\n", MotorRotationCount(OUT_A), MotorTachoCount(OUT_A), MotorActualSpeed(OUT_A));
  printf("OUT_B: %d %d %d\n", MotorRotationCount(OUT_B), MotorTachoCount(OUT_B), MotorActualSpeed(OUT_B));
  printf("OUT_C: %d %d %d\n", MotorRotationCount(OUT_C), MotorTachoCount(OUT_C), MotorActualSpeed(OUT_C));
  printf("OUT_D: %d %d %d\n", MotorRotationCount(OUT_D), MotorTachoCount(OUT_D), MotorActualSpeed(OUT_D));

  printf("Wait(SEC_1)\n");
  Wait(SEC_1);

  printf("RotateMotorNoWait(OUT_AB, 75, 7200)\n");
  RotateMotorNoWait(OUT_AB, 75, 7200);

  printf("OUT_A: %d %d %d\n", MotorRotationCount(OUT_A), MotorTachoCount(OUT_A), MotorActualSpeed(OUT_A));
  printf("OUT_B: %d %d %d\n", MotorRotationCount(OUT_B), MotorTachoCount(OUT_B), MotorActualSpeed(OUT_B));

  isBusy = MotorBusy(OUT_AB);
  printf("A or B isBusy = %d\n", (byte)isBusy);

  printf("Wait(SEC_20)\n");
  Wait(SEC_20);

  printf("OUT_A: %d %d %d\n", MotorRotationCount(OUT_A), MotorTachoCount(OUT_A), MotorActualSpeed(OUT_A));
  printf("OUT_B: %d %d %d\n", MotorRotationCount(OUT_B), MotorTachoCount(OUT_B), MotorActualSpeed(OUT_B));

  printf("resetting all tacho counters\n");
  ResetAllTachoCounts(OUT_ALL);

  printf("Wait(SEC_1)\n");
  Wait(SEC_1);

  printf("OnForSync(OUT_AB, SEC_10, 75)\n");
  OnForSync(OUT_AB, SEC_10, 75);

  for(i=0; i < 10; i++)
  {
    printf("OUT_A: %d %d %d\n", MotorRotationCount(OUT_A), MotorTachoCount(OUT_A), MotorActualSpeed(OUT_A));
    printf("OUT_B: %d %d %d\n", MotorRotationCount(OUT_B), MotorTachoCount(OUT_B), MotorActualSpeed(OUT_B));

    isBusy = MotorBusy(OUT_AB);
    printf("A or B isBusy = %d\n", (byte)isBusy);

    isBusy = MotorBusy(OUT_CD);
    printf("C or D isBusy = %d\n", (byte)isBusy);

    Wait(SEC_1);
  }

  printf("Wait(SEC_2)\n");
  Wait(SEC_2);

  printf("OUT_A: %d %d %d\n", MotorRotationCount(OUT_A), MotorTachoCount(OUT_A), MotorActualSpeed(OUT_A));
  printf("OUT_B: %d %d %d\n", MotorRotationCount(OUT_B), MotorTachoCount(OUT_B), MotorActualSpeed(OUT_B));

  // synchronized motor movement without tacho or time limitation
  printf("OnFwdSync(OUT_AB, 75)\n");
  OnFwdSync(OUT_AB, 75);

  printf("Wait(SEC_2)\n");
  Wait(SEC_2);

  printf("OUT_A: %d %d %d\n", MotorRotationCount(OUT_A), MotorTachoCount(OUT_A), MotorActualSpeed(OUT_A));
  printf("OUT_B: %d %d %d\n", MotorRotationCount(OUT_B), MotorTachoCount(OUT_B), MotorActualSpeed(OUT_B));

  printf("Off(OUT_AB)\n");
  Off(OUT_AB); // stop with braking

  printf("OUT_A: %d %d %d\n", MotorRotationCount(OUT_A), MotorTachoCount(OUT_A), MotorActualSpeed(OUT_A));
  printf("OUT_B: %d %d %d\n", MotorRotationCount(OUT_B), MotorTachoCount(OUT_B), MotorActualSpeed(OUT_B));

  printf("Wait(SEC_1)\n");
  Wait(SEC_1);

/*
 *  Turn ratio is how tight you turn and to what direction you turn
 *   - 0 value is moving straight forward
 *   - Negative values turns to the left
 *   - Positive values turns to the right
 *   - Value -100 stops the left motor
 *   - Value +100 stops the right motor
 *   - Values less than -100 makes the left motor run the opposite
 *     direction of the right motor (Spin)
 *   - Values greater than +100 makes the right motor run the opposite
 *     direction of the left motor (Spin)
 */

  printf("OUT_A: %d %d %d\n", MotorRotationCount(OUT_A), MotorTachoCount(OUT_A), MotorActualSpeed(OUT_A));
  printf("OUT_B: %d %d %d\n", MotorRotationCount(OUT_B), MotorTachoCount(OUT_B), MotorActualSpeed(OUT_B));

  printf("OnFwdSyncEx(OUT_AB, 75, -20, RESET_NONE)\n");
  OnFwdSyncEx(OUT_AB, 75, -20, RESET_NONE);

  printf("Wait(SEC_2)\n");
  Wait(SEC_2);

  printf("OUT_A: %d %d %d\n", MotorRotationCount(OUT_A), MotorTachoCount(OUT_A), MotorActualSpeed(OUT_A));
  printf("OUT_B: %d %d %d\n", MotorRotationCount(OUT_B), MotorTachoCount(OUT_B), MotorActualSpeed(OUT_B));

  printf("OnFwdSync(OUT_AB, 50, -50, RESET_NONE)");
  OnFwdSyncEx(OUT_AB, 50, -50, RESET_NONE);

  printf("Wait(SEC_2)\n");
  Wait(SEC_2);

  printf("OUT_A: %d %d %d\n", MotorRotationCount(OUT_A), MotorTachoCount(OUT_A), MotorActualSpeed(OUT_A));
  printf("OUT_B: %d %d %d\n", MotorRotationCount(OUT_B), MotorTachoCount(OUT_B), MotorActualSpeed(OUT_B));

  printf("OnFwdSync(OUT_AB, 20, -100, RESET_NONE)\n");
  OnFwdSyncEx(OUT_AB, 20, -100, RESET_NONE);

  printf("Wait(SEC_2)\n");
  Wait(SEC_2);

  printf("OUT_A: %d %d %d\n", MotorRotationCount(OUT_A), MotorTachoCount(OUT_A), MotorActualSpeed(OUT_A));
  printf("OUT_B: %d %d %d\n", MotorRotationCount(OUT_B), MotorTachoCount(OUT_B), MotorActualSpeed(OUT_B));

  printf("OnFwdSync(OUT_AB, 80, -150, RESET_NONE)\n");
  OnFwdSyncEx(OUT_AB, 80, -150, RESET_NONE);

  printf("Wait(SEC_2)\n");
  Wait(SEC_2);

  printf("OUT_A: %d %d %d\n", MotorRotationCount(OUT_A), MotorTachoCount(OUT_A), MotorActualSpeed(OUT_A));
  printf("OUT_B: %d %d %d\n", MotorRotationCount(OUT_B), MotorTachoCount(OUT_B), MotorActualSpeed(OUT_B));

  printf("OnFwdSync(OUT_AB, 30, -200, RESET_NONE)\n");
  OnFwdSyncEx(OUT_AB, 30, -200, RESET_NONE);

  printf("Wait(SEC_2)\n");
  Wait(SEC_2);

  printf("OUT_A: %d %d %d\n", MotorRotationCount(OUT_A), MotorTachoCount(OUT_A), MotorActualSpeed(OUT_A));
  printf("OUT_B: %d %d %d\n", MotorRotationCount(OUT_B), MotorTachoCount(OUT_B), MotorActualSpeed(OUT_B));

  printf("Off(OUT_AB)\n");
  Off(OUT_AB);

  printf("Wait(SEC_2)\n");
  Wait(SEC_2);

  printf("OUT_A: %d %d %d\n", MotorRotationCount(OUT_A), MotorTachoCount(OUT_A), MotorActualSpeed(OUT_A));
  printf("OUT_B: %d %d %d\n", MotorRotationCount(OUT_B), MotorTachoCount(OUT_B), MotorActualSpeed(OUT_B));

  printf("ResetAllTachoCounts(OUT_AB)\n");
  ResetAllTachoCounts(OUT_AB);

  printf("Wait(SEC_2)\n");
  Wait(SEC_2);

  printf("OUT_A: %d %d %d\n", MotorRotationCount(OUT_A), MotorTachoCount(OUT_A), MotorActualSpeed(OUT_A));
  printf("OUT_B: %d %d %d\n", MotorRotationCount(OUT_B), MotorTachoCount(OUT_B), MotorActualSpeed(OUT_B));

  // a blocking version of RotateMotor (function call does not return
  // until the motor rotation is complete)
  printf("RotateMotor(OUT_AB, 75, 1800)");
  RotateMotor(OUT_AB, 75, 1800); // 5 rotations

  // by the time the function above returns the motors are no longer busy
  isBusy = MotorBusy(OUT_AB);
  printf("A or B isBusy = %d\n", isBusy);

  // this call starts the motors running Forward without regulation or
  // synchronization and the function call does not return until the
  // specified amount of time has elapsed.
  // at the end of the elapsed time the motors are stopped with braking.
  printf("OnFor(OUT_AB, SEC_3)\n");
  OnFor(OUT_AB, SEC_3);

  printf("Off(OUT_AB)\n");
  Off(OUT_AB);

  printf("Wait(SEC_5)\n");
  Wait(SEC_5);

  OutputClose();
  OutputExit();
  printf("end of out_test\n");

  return 0;
}
