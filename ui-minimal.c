
#include "actrui.h"

[[clang::export_name("actr_init")]]
void actr_init()
{
    actr_ui_init(); // required
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
