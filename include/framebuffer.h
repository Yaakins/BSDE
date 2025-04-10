#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

#define WIDTH 1366
#define HEIGHT 768

#define BLACK 0xFF000000
#define WHITE 0xFFFFFFFF
#define RED   0xFFFF0000
#define GREEN 0xFF00FF00
#define BLUE  0xFF0000FF
#define CYAN  0xFF00FFFF

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif

typedef struct {
  int width;
  int height;
  uint32_t *data;
} FB_Texture;

typedef struct {
  int width;
  int height;
  uint32_t *buffers[2];
  int buffer_index;
  void *fb;
} FB_Display;

uint32_t FB_ColorValue(unsigned char a, unsigned char r, unsigned char g, unsigned char b);

FB_Texture *FB_CreateTexture(int width, int height);
void FB_DestroyTtexture(FB_Texture *t);

void FB_FillTexture(FB_Texture *t, uint32_t color);

FB_Display *FB_InitDisplay();
void FB_DestroyDisplay(FB_Display *d);

void FB_ClearDisplay(FB_Display *d, uint32_t color);
void FB_DisplayFlip(FB_Display *d);

void FB_BlitTextureDisplay(FB_Display *d, FB_Texture *t, int x, int y);

#endif
