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
#include <fcntl.h>
#include <string.h>
#include <libgen.h>
#include <limits.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>


#ifndef byte
#define byte unsigned char
#endif

#define LCD_WIDTH     178 // LCD horizontal pixels
#define LCD_HEIGHT    128 // LCD vertical pixels

#define LCD_MEM_WIDTH       60 // width of HW frame buffer in bytes
#define LCD_BUFFER_LENGTH   (LCD_MEM_WIDTH * LCD_HEIGHT)

#define LCD_BYTE_WIDTH      ((LCD_WIDTH + 7) / 8)
#define LCD_BUFFER_SIZE     (LCD_BYTE_WIDTH * LCD_HEIGHT)

#define LMS_LCD_DEVICE_NAME "/dev/fb0" // DISPLAY device file name

byte hwBuffer[LCD_BUFFER_LENGTH];
byte displayBuf[LCD_BUFFER_SIZE];

typedef enum {
  ifRAW_FB0,
  ifRAW_BUF,
  ifXBM,
  ifP1,
  ifP4,
  ifBMP,
  ifPNG
} ImageFormat;

byte UnPixelTab[] = {
// 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
   0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 6, // 1
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 2
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 3
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 4
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 5
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 6
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 7
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 8
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 9
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // A
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // B
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // C
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // D
   1, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // E
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 7  // F
  };

void frameBufferToLcd(byte* pSrc, byte* pDst)
{
  unsigned long Pixels;
  unsigned short X;
  unsigned short Y;

  for (Y = 0; Y < LCD_HEIGHT; Y++)
  {
    for (X = 0; X < 7; X++)
    {
      // read 8 bytes (3 bits per byte) into a 32-bit unsigned long from our source
      Pixels  = (unsigned long)UnPixelTab[*pSrc] << 0;  pSrc++;
      Pixels |= (unsigned long)UnPixelTab[*pSrc] << 3;  pSrc++;
      Pixels |= (unsigned long)UnPixelTab[*pSrc] << 6;  pSrc++;
      Pixels |= (unsigned long)UnPixelTab[*pSrc] << 9;  pSrc++;
      Pixels |= (unsigned long)UnPixelTab[*pSrc] << 12; pSrc++;
      Pixels |= (unsigned long)UnPixelTab[*pSrc] << 15; pSrc++;
      Pixels |= (unsigned long)UnPixelTab[*pSrc] << 18; pSrc++;
      Pixels |= (unsigned long)UnPixelTab[*pSrc] << 21; pSrc++;

      // now write the 24 bits to 3 8-bit bytes in our destination
      *pDst = (byte)(Pixels >> 0);  pDst++;
      *pDst = (byte)(Pixels >> 8);  pDst++;
      *pDst = (byte)(Pixels >> 16); pDst++;
    }
    Pixels  = (unsigned long)UnPixelTab[*pSrc] << 0;  pSrc++;
    Pixels |= (unsigned long)UnPixelTab[*pSrc] << 3;  pSrc++;
    Pixels |= (unsigned long)UnPixelTab[*pSrc] << 6;  pSrc++;
    Pixels |= (unsigned long)UnPixelTab[*pSrc] << 9;  pSrc++;

    *pDst = (byte)(Pixels >> 0);  pDst++;
    *pDst = (byte)(Pixels >> 8);  pDst++;
  }
}

static const unsigned char reverse_bits_table[] = {
  0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0, 0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
  0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8, 0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
  0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4, 0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
  0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec, 0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
  0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2, 0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
  0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea, 0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
  0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6, 0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
  0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee, 0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
  0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1, 0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
  0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9, 0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
  0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5, 0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
  0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed, 0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
  0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3, 0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
  0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb, 0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
  0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7, 0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
  0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef, 0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff,
};

byte reverse_bits(byte b)
{
  return reverse_bits_table[b];
}

byte GetPixelColor(byte b, byte bit)
{
  return (b & (1 << bit)) ? 1 : 0;
}

int SaveFrameBufferToFile(ImageFormat fmt, byte* data, char* filename)
{
  byte width = LCD_WIDTH;
  byte height = LCD_HEIGHT;
  int dataLength = LCD_BUFFER_LENGTH;
  char fullname[128];
  char* fn_copy = strdup(filename);
  char *base, *path;
  path = dirname(fn_copy);
  base = basename(filename);

  byte len = strlen(base);
  // check for and remove extension if it exists
  char* ext = strrchr(base, '.');
  if (ext) {
    len = ext-base;
  }
  // now build full path
  strcpy(fullname, path);
  strcat(fullname, "/");
  strncat(fullname, base, len);
  switch(fmt)
  {
    case ifRAW_FB0:
    case ifRAW_BUF:
      strcat(fullname, ".raw");
      break;
    case ifP1:
    case ifP4:
      strcat(fullname, ".pbm");
      break;
    case ifBMP:
      strcat(fullname, ".bmp");
      break;
    case ifPNG:
      strcat(fullname, ".png");
      break;
    default:
      // default is XBM
      strcat(fullname, ".xbm");
      break;
  }

  if (fmt != ifRAW_FB0)
  {
    // convert to display buffer
    frameBufferToLcd(data, displayBuf);
    // point data to new buffer
    data = displayBuf;
    dataLength = LCD_BUFFER_SIZE; // display buffer size aka sizeof(displayBuf)
  }

  FILE * pFile = NULL;
  pFile = fopen(fullname, "w");

  if (pFile != NULL)
  {
    int i, j, k, cnt, bw;
    bw = ((width + 7) / 8);
    switch (fmt)
    {
      case ifRAW_FB0:
      case ifRAW_BUF:
        // just write out the raw bytes
        fwrite(data, 1, dataLength, pFile);
        break;
      case ifP1:
        fprintf(pFile, "P1\n%d %d\n", width, height);
        for (i = 0; i < height; i++)
        {
          byte b;
          for (j = 0; j < bw-1; j++)
          {
            // get a byte
            b = data[i*bw+j];
//            b = reverse_bits(data[i*bw+j]);
            // output 8 bits per byte except for last byte per line
            for (k = 0; k < 8; k++)
//            for (k = 7; k >= 0; k--)
            {
              fprintf(pFile, "%c ", (b & (1 << k)) ? '1' : '0');
            }
          }
          // handle last byte per line
          b = data[i*bw+(bw-1)];
//          b = reverse_bits(data[i*bw+(bw-1)]);
          // how many bits are extra?
          cnt = 8 - (bw*8 - width); // e.g., 2 if width = 178
//          cnt = bw*8 - width; // e.g., 6 if width = 178
          for (k = 0; k < cnt; k++)
//          for (k = 7; k >= cnt; k--)
          {
            fprintf(pFile, "%c ", (b & (1 << k)) ? '1' : '0');
          }
          fprintf(pFile, "\n");
        }
        break;
      case ifP4:
        fprintf(pFile, "P4\n%d %d\n", width, height);
        cnt = bw*height;
        for (i=0; i < cnt; i++)
        {
//          byte b = data[i];
          byte b = reverse_bits(data[i]);
          fwrite(&b, 1, 1, pFile);
        }
        break;
      case ifBMP:
        printf("PNG image format is not yet supported\n");
        break;
      case ifPNG:
        printf("BMP image format is not yet supported\n");
        break;
      default:
        // XBM format
        fprintf(pFile, "#define %s_width %d\n", base, width);
        fprintf(pFile, "#define %s_height %d\n", base, height);
        fprintf(pFile, "static char %s_bits[] = {\n", base);
        cnt = bw*height;
        for (i=0; i < cnt; i++)
        {
          fprintf(pFile, "0x%2.2X,", data[i]);
          if ((i+1) % 12 == 0)
            fprintf(pFile, "\n");
        }
        fprintf(pFile, " };\n");
        break;
    }
    fclose(pFile);
    return 0;
  }
  return 1;
}

// snapshot -ofilename -fN
int main(int argc, char *argv[])
{
  char dest[PATH_MAX];
  char *path;
  char fullname[PATH_MAX];
  if (readlink( "/proc/self/exe", dest, PATH_MAX) == -1)
    return 1;

  path = dirname(dest);
  // check arguments
  ImageFormat fmt = ifRAW_FB0;
  strcpy(fullname, path);
  strcat(fullname, "/snapshot.raw");
  char *filename = fullname;
  int index;
  int c;

  opterr = 0;
  while ((c = getopt (argc, argv, "o:f:")) != -1)
    switch (c)
    {
      case 'o':
        filename = optarg;
        break;
      case 'f':
        fmt = (ImageFormat)atoi(optarg);
        break;
      case '?':
        return 1;
    }
  byte *pTmp, *pFB0;
  int DispFile = open(LMS_LCD_DEVICE_NAME, O_RDWR);
  if (DispFile >= 0)
  {
    pTmp = (byte*)mmap(NULL, LCD_BUFFER_LENGTH, PROT_READ + PROT_WRITE, MAP_SHARED, DispFile, 0);
    if (pTmp == MAP_FAILED)
    {
      close(DispFile);
      return 1;
    }
    else
    {
      pFB0 = pTmp;
      // read from frame buffer into hwBuffer
      memmove((void *)hwBuffer, (const void*)pFB0, LCD_BUFFER_LENGTH);
      // now close everything we opened
      munmap(pFB0, LCD_BUFFER_LENGTH);
      close(DispFile);

      // now save hwBuffer to a file in the desired format
      return SaveFrameBufferToFile(fmt, hwBuffer, filename);
    }
  }
  return 1;
}

