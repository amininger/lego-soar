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

#include "ev3_lcd.h"
#include "ev3_command.h"

int main()
{
  int i;
  LcdInit();
  printf("start of lcd_test\n");
  printf("CircleOut(50, 40, 10);\n");
  printf("CircleOutEx(30, 24, 10, DRAW_OPT_FILL_SHAPE);\n");
  printf("LineOut(10, 10, 80, 60);\n");
  printf("RectOut(20, 20, 40, 40);\n");
  printf("RectOutEx(65, 25, 20, 30, DRAW_OPT_FILL_SHAPE);\n");
  printf("EllipseOut(70, 30, 15, 20);\n");

  CircleOut(50, 40, 10);
  CircleOutEx(30, 24, 10, DRAW_OPT_FILL_SHAPE);
  LineOut(10, 10, 80, 60);
  RectOut(20, 20, 40, 40);
  RectOutEx(65, 25, 20, 30, DRAW_OPT_FILL_SHAPE);
  EllipseOut(70, 30, 15, 20);

  Wait(SEC_1);

  printf("LcdBmpFile(1, 0, 64, \"/media/card/mindstorms.rgf\");\n");
  LcdBmpFile(1, 0, 64, "/media/card/mindstorms.rgf");
  Wait(SEC_1);

  for (i = 0; i < 35; i++)
  {
    printf("LcdIcon(1, 130, 64, ICONTYPE_NORMAL, 0);\n");
    LcdIcon(1, 0+((i % 7)*24), 0+((i / 7) * 12), ICONTYPE_NORMAL, i);
    Wait(250);
  }

  printf("LcdText(1, 0, 100, \"hello world\");\n");
  LcdText(1, 0, 100, "hello world");
  Wait(SEC_1);

  printf("LcdSelectFont(FONTTYPE_TINY);\n");
  LcdSelectFont(FONTTYPE_TINY);

  printf("LcdText(1, 100, 100, \"testing\");\n");
  LcdText(1, 100, 100, "testing");
  Wait(SEC_1);
  
  // these functions will replace any file extension included in
  // the filename with the "correct" extension based on the
  // specified image format
/*
  printf("LcdWriteDisplayToFile(\"/media/card/display.xbm\", ifXBM);\n");
  LcdWriteDisplayToFile("/media/card/display.xbm", ifXBM);

  printf("LcdWriteFrameBufferToFile(\"/media/card/fbuffer.xbm\", ifXBM);\n");
  LcdWriteFrameBufferToFile("/media/card/fbuffer.xbm", ifXBM);

  printf("LcdWriteDisplayToFile(\"/media/card/display_p1.pbm\", ifP1);\n");
  LcdWriteDisplayToFile("/media/card/display_p1.pbm", ifP1);

  printf("LcdWriteFrameBufferToFile(\"/media/card/fbuffer_p1.pbm\", ifP1);\n");
  LcdWriteFrameBufferToFile("/media/card/fbuffer_p1.pbm", ifP1);

  printf("LcdWriteDisplayToFile(\"/media/card/display_p4.pbm\", ifP4);\n");
  LcdWriteDisplayToFile("/media/card/display_p4.pbm", ifP4);

  printf("LcdWriteFrameBufferToFile(\"/media/card/fbuffer_p4.pbm\", ifP4);\n");
  LcdWriteFrameBufferToFile("/media/card/fbuffer_p4.pbm", ifP4);
*/
//  Wait(SEC_5);
  LcdExit();
  printf("end of lcd_test\n");
}
