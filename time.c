#include "actrwasm.h"
#include "actrcanvas.h"
#include "actrtime.h"

[[clang::export_name("actr_init")]]
void actr_init()
{
  char *time = actr_time_string();
  actr_canvas2d_fill_style(0, 0, 0, 100);          // set fill color to black
  actr_canvas2d_fill_rect(-10, -10, 9999, 9999);   // clear the canvas
  actr_canvas2d_fill_style(255, 255, 255, 100);    // set fill color to white
  actr_canvas2d_fill_text(25, 25, time); /// draw text a specified x/y position
}
