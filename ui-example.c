#include "actrui.h"

[[clang::export_name("actr_init")]]
void actr_init()
{
    actr_ui_init();

    for (int i = 0; i < 5; i++) {
        actr_ui_button(10, 20 + i * 30, 200, 25, "Hello Button");

    }
}

[[clang::export_name("actr_tap")]]
void actr_tap(int x, int y)
{
    actr_ui_tap(x, y);
}
[[clang::export_name("actr_move")]]
void actr_move(int x, int y)
{
    actr_ui_move(x, y);
}

// [[clang::export_name("actr_move")]] void actr_move(int x, int y) { }
[[clang::export_name("actr_step")]]
void actr_step()
{
    actr_ui_draw();
}
