#include <stdio.h>
#include <framebuffer.h>
#include <touchpad_input.h>

int main() {
  FB_Display *display = FB_InitDisplay();
  FB_Texture *cursor_cyan = FB_CreateTexture(10, 10);
  FB_FillTexture(cursor_cyan, CYAN);
  FB_Texture *cursor_green = FB_CreateTexture(10, 10);
  FB_FillTexture(cursor_green, GREEN);
  TP_Context *ctx = TP_CreateContext("/dev/input/event11");
  bool clicked = false;

  while (1) {
    FB_ClearDisplay(display, BLACK);
    TP_UpdateMousePos(ctx);
    //printf("mouse at {%d, %d}\n", ctx->mouse_pos.x, ctx->mouse_pos.y);
    FB_BlitTextureDisplay(display, (ctx->clicked ? cursor_green : cursor_cyan), ctx->mouse_pos.x, ctx->mouse_pos.y);
    FB_DisplayFlip(display);
  }
  return 0;
}
