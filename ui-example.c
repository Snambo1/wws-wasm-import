#include "actrui.h"

struct ActrVector *buttonList;
int tapCount = 0;
[[clang::export_name("actr_init")]]
void actr_init()
{
    buttonList = actr_vector_init(8,8);
    actr_ui_init();
    int id = 0;
    for (int i = 0; i < 25; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            struct ActrUIButton *button = actr_ui_button(10 + j * 120, 20 + i * 30, 110, 25, "");
            button->text = actr_sprintf("Button %i", (int[]){button->identity}, 4);
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
        button->text = actr_sprintf("Button %i got tap %i", (int[]){identity, tapCount}, 8);
    }
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
    // float data[] = {1.0f, 2.3f, 4.0f};

    // text = actr_sprintf("Hello World %f %f %f", data, sizeof(data));
    // actr_canvas2d_fill_text(10, 200, text);
    // actr_free(text);
}
