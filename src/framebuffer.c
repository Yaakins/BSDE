#include "framebuffer.h"

uint32_t FB_ColorValue(unsigned char a, unsigned char r, unsigned char g, unsigned char b) {
  return a << 24 | r << 16 | g << 8 | b;
}

static void *map_framebuffer_address(int size) {
  int fd = open("/dev/fb0", O_RDWR);
  if (fd < 0) {
    perror("Error while opening framebuffer device : ");
    return NULL;
  }
  void *fb_ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (fb_ptr == NULL) {
    perror("Error while mapping framebuffer device : ");
    return NULL;
  }
  return fb_ptr;
}

FB_Texture *FB_CreateTexture(int width, int height) {
  FB_Texture *res = malloc(sizeof(FB_Texture));
  if (!res) {
    perror("Error while allocating texture: ");
    return NULL;
  }
  res->width = width;
  res->height = height;
  res->data = malloc(sizeof(uint32_t) * width * height);
  if (!res->data) {
    perror("Error while allocating texture content: ");
    free(res);
    return NULL;
  }
  return res;
}

void FB_DestroyTexture(FB_Texture *t) {
  if (t) {
    free(t->data);
    free(t);
  }
  return;
}

void FB_FillTexture(FB_Texture *t, uint32_t color) {
  for (int i = 0; i < t->width * t->height; i++) {
    memcpy((t->data + i), &color, sizeof(uint32_t));
  }
  return;
}

void FB_ClearDisplay(FB_Display *d, uint32_t color) {
  for (int i = 0; i < d->width * (d->height); i++) {
    memcpy((d->buffers[d->buffer_index] + i), &color, sizeof(uint32_t));
  } 
  return;
}

FB_Display *FB_InitDisplay() {
  if (getenv("TERM") && strcmp(getenv("TERM"), "linux") != 0) {
    fprintf(stderr, "Unable to run with a graphic environment\n");
    exit(-1);
  }

  FB_Display *res = malloc(sizeof(FB_Display));
  if (!res) {
    perror("Error while allocating display: ");
    return NULL;
  }
  res->width = WIDTH;
  res->height = HEIGHT;
  res->buffer_index = 0;
  uint32_t *buf1 = malloc(sizeof(uint32_t) * WIDTH * (HEIGHT));
  uint32_t *buf2 = malloc(sizeof(uint32_t) * WIDTH * (HEIGHT));
  if (!buf1 || !buf2) {
    perror("Error while allocating buffers: ");
    free(res);
    free(buf1);
    free(buf2);
    return NULL;
  }
  res->buffers[0] = buf1;
  res->buffers[1] = buf2;
  res->fb = map_framebuffer_address(WIDTH * HEIGHT * sizeof(uint32_t));

  struct termios tinfo;
  tcgetattr(STDIN_FILENO, &tinfo);
  tinfo.c_lflag ^= ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &tinfo);

  return res;
}

void FB_DestroyDisplay(FB_Display *d) {
  if (d) {
    free(d->buffers[0]);
    free(d->buffers[1]);
    free(d);
  }
  return;
}

void FB_DisplayFlip(FB_Display *d) { 
  memcpy(d->fb, d->buffers[d->buffer_index], d->width * d->height * sizeof(uint32_t));
  d->buffer_index = (d->buffer_index + 1) % 2;
  return;
}

void FB_BlitTextureDisplay(FB_Display *d, FB_Texture *t, int x, int y) {
  /*if (x+t->width >= WIDTH || x < 0 || y < 0 || y + t->height >= HEIGHT) {
    fprintf(stderr, "Cannot blit outside the screen\n");
    return;
  }*/
  for (int i = 0; i < t->height; i++) {
    memcpy(d->buffers[d->buffer_index] + x + (y + i) * (WIDTH + 10), t->data + (i * t->height), sizeof(uint32_t) * t->width);
  }
  return;
}
