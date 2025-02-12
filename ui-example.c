#include "actrui.h"
#include "actrformat.h"
#include "actrtime.h"

struct ActrVector *buttonList;
int tapCount = 0;
[[clang::export_name("actr_init")]]
void actr_init()
{
    buttonList = actr_vector_init(8, 8);
    actr_ui_init();
    int id = 0;
    int width = 250;
    int height = 25;
    int pad = 10;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            struct ActrUIButton *button = actr_ui_button(10 + j * (width + pad), 20 + i * (height + pad), width, height, "");
            struct ActrFormatState *state = actr_format("Button %s");
            actr_format_int(state, button->identity);
            button->text = actr_format_close(state);
            actr_vector_add(buttonList, button);    
        }
    }
}

[[clang::export_name("actr_tap")]]
void actr_tap(int x, int y)
{
    tapCount++;
    int identity = actr_ui_tap(x, y);
    if (identity >= 0)
    {
        struct ActrUIButton *button = buttonList->head[identity];
        actr_free(button->text);
        struct ActrFormatState *state = actr_format("Button %s got tap %s @ %s");
        actr_format_int(state, button->identity);
        actr_format_int(state, tapCount);
        actr_format_int(state, actr_time());
        button->text = actr_format_close(state);
    }
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
