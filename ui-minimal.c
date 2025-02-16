
#include "actrui.h"
#include "actrlog.h"

struct ActrUIControlText * textbox;
struct ActrUIControlButton * button;

[[clang::export_name("actr_init")]]
void actr_init()
{
    actr_ui_init(); // required
    textbox = actr_ui_text(100, 100, 500, 25, "Hello World");
    button = actr_ui_button(100, 200, 250, 25, "My Button");
}

[[clang::export_name("actr_pointer_tap")]]
void actr_pointer_tap(int x, int y)
{
    actr_ui_tap(x, y);
}

[[clang::export_name("actr_pointer_move")]]
void actr_pointer_move(int x, int y)
{
    actr_ui_move(x, y);
}

[[clang::export_name("actr_key_down")]]
void actr_key_down(int key)
{
    actr_ui_key_down(key);
}

[[clang::export_name("actr_resize")]]
void actr_resize()
{
    actr_ui_invalidate(); // required
}

[[clang::export_name("actr_step")]]
void actr_step(double delta)
{
    actr_ui_draw(delta); // required


}
