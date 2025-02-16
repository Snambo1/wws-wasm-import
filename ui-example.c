#include "actrformat.h"
#include "actrtime.h"
#include "actrui.h"

[[clang::export_name("actr_init")]]
void actr_init()
{
  actr_ui_init();
}

[[clang::export_name("actr_key_down")]]
void actr_key_down(char key)
{
  actr_ui_key_down(key);
}
[[clang::export_name("actr_tap")]]
void actr_tap(int x, int y)
{
  int identity = actr_ui_tap(x, y);
  if (identity > 0)
  {
    actr_ui_remove_control(identity);
  }
  else
  {
    actr_ui_button(x - 10, y - 10, 20, 20, "X");
  }
  // press_button(actr_ui_tap(x, y));
}

[[clang::export_name("actr_move")]]
void actr_move(int x, int y)
{
  actr_ui_move(x, y);
}

int removeIdentity = 0;
int remsteps = 0;
int countdown = 30;
extern void total(long long n);
extern void remstep(int n);
extern void remid(int n);
extern void removing(int n);
[[clang::export_name("actr_step")]]
void actr_step(double delta)
{
  actr_ui_draw(delta);
}
