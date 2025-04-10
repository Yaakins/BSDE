#include <stdio.h>
#include <framebuffer.h>
#include <touchpad_input.h>

int main() {
  FB_Display *display = FB_InitDisplay();
  FB_Texture *cursor = FB_CreateTexture(10, 10);
  FB_FillTexture(cursor, CYAN);
  TP_Context *ctx = TP_CreateContext("/dev/input/event11");

  while (1) {
    FB_ClearDisplay(display, BLACK);
    TP_UpdateMousePos(ctx);
    //printf("mouse at {%d, %d}\n", ctx->mouse_pos.x, ctx->mouse_pos.y);
    FB_BlitTextureDisplay(display, cursor, ctx->mouse_pos.x, ctx->mouse_pos.y);
    FB_DisplayFlip(display);
  }
  return 0;
}
