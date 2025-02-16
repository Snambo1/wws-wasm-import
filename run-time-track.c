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
}

[[clang::export_name("actr_tap")]]
void actr_tap(int x, int y)
{
    
}

[[clang::export_name("actr_move")]]
void actr_move(int x, int y)
{
}

[[clang::export_name("actr_async_result")]]
void actr_async_result(int handle, enum AsyncResult success)
{
    
}

[[clang::export_name("actr_step")]]
void actr_step(double delta)
{
}
