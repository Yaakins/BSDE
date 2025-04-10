#include "touchpad_input.h"

const float x_factor = (float)SCREEN_WIDTH/(float)TOUCHPAD_WIDTH;
const float y_factor = (float)SCREEN_HEIGHT/(float)TOUCHPAD_HEIGHT;

void read_event_sequence(TP_Context *context, TP_Event *a_event) {
  static TP_Vector position;
  struct input_event event = {.type = EV_MSC};
  a_event->type = TP_EVENT_ABSPOS;
  while (event.type != EV_SYN || event.code != SYN_REPORT) {
    ssize_t b_read = read(context->fd, &event, 24);
    if (b_read == 24) {
      if (event.type == EV_ABS) {
        if (event.code == ABS_MT_POSITION_X || event.code == ABS_X) {
          position.x = event.value;
        } else if (event.code == ABS_MT_POSITION_Y || event.code == ABS_Y) {
          position.y = event.value;
        }
      } else if (event.type == EV_KEY) {
        if (event.code == BTN_LEFT) {
          if (event.value == 0) {
            a_event->type = TP_EVENT_BTNUP;
          } else {
            a_event->type = TP_EVENT_BTNDOWN;
          }
        } else if (event.code == BTN_RIGHT) {
          printf("RGIHT\n");
        } else if (event.code == BTN_TOUCH || event.code != BTN_TOOL_FINGER) {
          if (event.value == 0) {
            a_event->type = TP_EVENT_TOUCHUP;
          } else {
            a_event->type = TP_EVENT_TOUCHDOWN;
          }
        }
      }
    } else {
      printf("error while reading event\n");
      return;
    }
  }
  a_event->pos = position;
  return;
}

void TP_UpdateMousePos(TP_Context *ctx) {
  TP_Event event;
  while (TP_PollEvent(ctx, &event)) {
    if (event.type == TP_EVENT_ABSPOS) {
      TP_Vector rel = {.x = event.pos.x - ctx->last_tp_pos.x, .y = event.pos.y - ctx->last_tp_pos.y};
      ctx->last_tp_pos = event.pos;

      ctx->mouse_pos.x += rel.x * x_factor;
      ctx->mouse_pos.y += rel.y * y_factor;
    } else if (event.type == TP_EVENT_TOUCHDOWN) {
      ctx->last_tp_pos = event.pos;
    }
    if (ctx->mouse_pos.x < 0) ctx->mouse_pos.x = 0;
    if (ctx->mouse_pos.y < 0) ctx->mouse_pos.y = 0;
    if (ctx->mouse_pos.x > SCREEN_WIDTH) ctx->mouse_pos.x = SCREEN_WIDTH;
    if (ctx->mouse_pos.y > SCREEN_HEIGHT) ctx->mouse_pos.y = SCREEN_HEIGHT;
  }
  return;
}

bool TP_PollEvent(TP_Context *context, TP_Event *event) {
  struct pollfd pfd = {.fd = context->fd, .events = POLLIN};
  int available = poll(&pfd, 1, 0);
  if (available == -1) {
    perror("I/O error while polling events : ");
  } else if (available == 0) {
    return false;
  } else {
    if (pfd.revents & POLLIN) {
      read_event_sequence(context, event);
      return true;
    } else {
      return false;
    }
  }
}


TP_Context *TP_CreateContext(const char *input_path) {
  int input_fd = open(input_path, O_RDONLY | O_NONBLOCK);
  TP_Context *ctx = malloc(sizeof(TP_Context));
  if (!ctx) {
    perror("Unable to create a context: ");
  }
  ctx->fd = input_fd;
  ctx->mouse_pos = (TP_Vector){0};
  ctx->last_tp_pos = (TP_Vector){0};
  return ctx;
}

void TP_DestroyContext(TP_Context *ctx) {
  if (ctx) free(ctx);
  return;
}
