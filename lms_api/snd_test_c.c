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
#include "ev3_sound.h"

Tone sweepUp[] = {
  {TONE_C4, MS_50},
  {TONE_E4, MS_50},
  {TONE_G4, MS_50},
  {TONE_C5, MS_50},
  {TONE_E5, MS_50},
  {TONE_G5, MS_50},
  {TONE_C6, MS_200}
};

int main()
{
  SoundInit();
  printf("start of pt_test\n");

  printf("PlayTone(TONE_A4, MS_500)\n");
  PlayTone(TONE_A4, MS_500);
  Wait(SEC_1);

  printf("PlaySound(SOUND_FAST_UP)\n");
  PlaySound(SOUND_FAST_UP);
  Wait(SEC_1);

  printf("PlayTones(sweepUp)\n");
  PlayTones(sweepUp);
  Wait(SEC_1);

  printf("PlayFile('/media/card/piano.rmd')\n");
  PlayFile("/media/card/piano.rmd");
  Wait(SEC_2);

  printf("PlayFile('/media/card/1.rsf')\n");
  PlayFile("/media/card/1.rsf");
  Wait(SEC_2);

  printf("PlayFile('/media/card/2.rsf') clapping\n");
  PlayFile("/media/card/2.rsf");
  Wait(SEC_2);

  printf("PlayFile('/media/card/3.rsf') eating\n");
  PlayFile("/media/card/3.rsf");
  Wait(SEC_2);

  printf("PlayFile('/media/card/4.rsf') laugh\n");
  PlayFile("/media/card/4.rsf");
  Wait(SEC_2);

  printf("PlayFile('/media/card/5.rsf') growl\n");
  PlayFile("/media/card/5.rsf");
  Wait(SEC_2);

  printf("PlayFile('/media/card/6.rsf') whine\n");
  PlayFile("/media/card/6.rsf");
  Wait(SEC_2);

  printf("PlayFile('/media/card/7.rsf') elephant\n");
  PlayFile("/media/card/7.rsf");
  Wait(SEC_2);

  printf("PlayFile('/media/card/8.rsf') ratchet\n");
  PlayFile("/media/card/8.rsf");
  Wait(SEC_2);

  printf("PlayFile('/media/card/9.rsf')\n");
  PlayFile("/media/card/9.rsf");
  Wait(SEC_2);

  printf("PlayFile('/media/card/10.rsf')\n");
  PlayFile("/media/card/10.rsf");
  Wait(SEC_2);

  printf("PlayFile('/media/card/11.rsf')\n");
  PlayFile("/media/card/11.rsf");
  Wait(SEC_2);

  printf("PlayFile('/media/card/12.rsf') tada\n");
  PlayFile("/media/card/12.rsf");
  Wait(SEC_2);

  printf("PlayFile('/media/card/13.rsf')\n");
  PlayFile("/media/card/13.rsf");
  Wait(SEC_2);

  printf("PlayFile('/media/card/14.rsf') snore\n");
  PlayFile("/media/card/14.rsf");
  Wait(SEC_2);

  printf("PlayFile('/media/card/15.rsf')\n");
  PlayFile("/media/card/15.rsf");
  Wait(SEC_2);

  printf("PlayFile('/media/card/16.rsf')\n");
  PlayFile("/media/card/16.rsf");
  Wait(SEC_2);

  printf("PlayFile('/media/card/17.rsf')\n");
  PlayFile("/media/card/17.rsf");
  Wait(SEC_2);

  printf("PlayFile('/media/card/test3.wav') (evil laugh)\n");
  PlayFile("/media/card/test3.wav");
  Wait(SEC_2);

  printf("PlayFile('/media/card/test1.wav') (> 64k)\n");
  PlayFile("/media/card/test1.wav");
  Wait(SEC_2);

  printf("end of pt_test\n");
}
