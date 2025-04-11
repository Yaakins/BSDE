#ifndef _TOUCHPAD_INPUT_
#define _TOUCHPAD_INPUT_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <stdbool.h>

#define TOUCHPAD_WIDTH 2856
#define TOUCHPAD_HEIGHT 1616

#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768

extern const float x_factor;
extern const float y_factor;

typedef struct {
  int x;
  int y;
} TP_Vector;

typedef struct {
  int fd;
  TP_Vector mouse_pos;
  TP_Vector last_tp_pos;
  bool clicked;
} TP_Context;

typedef enum {
  TP_EVENT_ABSPOS,
  TP_EVENT_BTNDOWN,
  TP_EVENT_BTNUP,
  TP_EVENT_TOUCHDOWN,
  TP_EVENT_TOUCHUP
} TP_EventType;

typedef struct {
  TP_EventType type;
  TP_Vector pos;
} TP_Event;

bool TP_PollEvent(TP_Context *context, TP_Event *event);
void TP_UpdateMousePos(TP_Context *context);

TP_Context *TP_CreateContext(const char *input_path);
void TP_DestroyContext(TP_Context *ctx);

#endif
