#include "actrformat.h"
#include "actrtime.h"
#include "actrui.h"

struct ActrVector *controlList;
int tapCount = 0;
[[clang::export_name("actr_init")]]
void actr_init()
{
  controlList = actr_vector_init(8, 8);
  actr_ui_init();
  int id = 0;
  int width = 250;
  int height = 25;
  int pad = 10;

  struct ActrUIControlButton *button = actr_ui_button(10, 200, width, height, "");
  struct ActrFormatState *state = actr_format("Button %s");
  actr_format_int(state, button->identity);
  button->label = actr_format_close(state);
  actr_vector_add(controlList, button);

  struct ActrUIControlText *text = actr_ui_text(10, 100, 500, height, actr_heap_string("hello world"));
  actr_vector_add(controlList, text);
}
void press_button(int identity)
{
  if (identity > 0)
  {
    tapCount++;
    struct ActrUIControlButton *button = controlList->head[identity - 1];
    if (button->type != ActrUITypeButton) return;
    actr_free(button->label);
    struct ActrFormatState *state = actr_format("Button %s got tap %s @ %s");
    actr_format_int(state, button->identity);
    actr_format_int(state, tapCount);
    actr_format_int(state, actr_time());
    button->label = actr_format_close(state);
  }
}
[[clang::export_name("actr_key_down")]]
void actr_key_down(char key)
{
  int identity = actr_ui_key_down(key);
  if (key == 32)
  {
    press_button(identity);
  }
}
[[clang::export_name("actr_tap")]]
void actr_tap(int x, int y)
{
  press_button(actr_ui_tap(x, y));
}

[[clang::export_name("actr_move")]]
void actr_move(int x, int y)
{
  actr_ui_move(x, y);
}

[[clang::export_name("actr_step")]]
void actr_step(double delta)
{
  actr_ui_draw(delta);
}
