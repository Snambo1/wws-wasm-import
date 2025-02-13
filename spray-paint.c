

#include "actrformat.h"
#include "actrtime.h"
#include "actrui.h"
#include "actrprng.h"
// black,

struct MyPixel
{
    struct ActrPoint point;
    struct ActrSize size;
    unsigned int color;
};
struct MyState
{
    int pointerDown;
    struct ActrVector *results;
    int whiteButton;
    int redButton;
    int greenButton;
    int yellowButton;
    int blueButton;
    int brownButton;
    int orangeButton;
    int pinkButton;
    int purpleButton;
    int grayButton;
    int deleteButton;
    unsigned int color;
    int delete;
};

struct MyState *state;

[[clang::export_name("actr_init")]]
void actr_init()
{

    int width = 70;
    int height = 20;
    int top = 0;
    actr_ui_init();
    state = actr_malloc(sizeof(struct MyState));
    state->whiteButton = actr_ui_button(10, top += 50, width, 20, "WHITE")->identity;
    state->redButton = actr_ui_button(10, top += 50, width, 20, "RED")->identity;
    state->greenButton = actr_ui_button(10, top += 50, width, 20, "GREEN")->identity;
    state->yellowButton = actr_ui_button(10, top += 50, width, 20, "YELLOW")->identity;
    state->blueButton = actr_ui_button(10, top += 50, width, 20, "BLUE")->identity;
    state->brownButton = actr_ui_button(10, top += 50, width, 20, "BROWN")->identity;
    state->orangeButton = actr_ui_button(10, top += 50, width, 20, "ORANGE")->identity;
    state->pinkButton = actr_ui_button(10, top += 50, width, 20, "PINK")->identity;
    state->purpleButton = actr_ui_button(10, top += 50, width, 20, "PURPLE")->identity;
    state->grayButton = actr_ui_button(10, top += 50, width, 20, "GRAY")->identity;
    state->deleteButton = actr_ui_button(10, top += 50, width, 20, "DELETE")->identity;
    state->color = actr_pack_bytes(255,255,255,100);
    state->results = actr_vector_init(4, 4);
}

extern void setcolor(int i);
[[clang::export_name("actr_pointer_tap")]]
void actr_pointer_tap(int x, int y)
{
    int id = actr_ui_tap(x, y);
    state->delete = 0;
    if (id == state->whiteButton)
    {
        actr_log("set white");
        state->color = actr_pack_bytes(255, 255, 255, 100);
    }
    else if (id == state->redButton)
    {
        actr_log("set red");
        state->color = actr_pack_bytes(255, 0, 0, 100);
    }
    else if (id == state->greenButton)
    {
        actr_log("set green");
        state->color = actr_pack_bytes(0, 255, 0, 100);
    }
    else if (id == state->yellowButton)
    {
        state->color = actr_pack_bytes(255, 255, 64, 100);
    }
    else if (id == state->blueButton)
    {
        state->color = actr_pack_bytes(0, 0, 255, 100);
    }
    else if (id == state->brownButton)
    {
        state->color = actr_pack_bytes(128, 64, 32, 100);
    }
    else if (id == state->orangeButton)
    {
        state->color = actr_pack_bytes(255, 128, 0, 100);
    }
    else if (id == state->pinkButton)
    {
        state->color = actr_pack_bytes(255, 0, 128, 100);
    }
    else if (id == state->purpleButton)
    {
        state->color = actr_pack_bytes(255, 0, 255, 100);
    }
    else if (id == state->grayButton)
    {
        state->color = actr_pack_bytes(128, 128, 128, 100);
    }
    else if (id == state->deleteButton)
    {
        state->delete = 1;
    }
}
[[clang::export_name("actr_pointer_down")]]
void actr_pointer_down(int x, int y)
{
    state->pointerDown = 1;
}

[[clang::export_name("actr_pointer_up")]]
void actr_pointer_up(int x, int y)
{
    state->pointerDown = 0;
}

[[clang::export_name("actr_pointer_move")]]
void actr_pointer_move(int x, int y)
{
    actr_ui_move(x, y);
    if (!state->pointerDown)
    {
        return;
    }
    struct ActrQuadTreeBounds bounds;
    int size = 50;
    for (int i = 0; i < 100; i++)
    {
        bounds.point.x = x + actr_prng() * size - size / 2;
        bounds.point.y = y + actr_prng() * size - size / 2;
        bounds.size.w = 3 + actr_prng() * 10;
        bounds.size.h = 3 + actr_prng() * 10;

        // actr_quad_tree_query(state->tree, &bounds, state->results);
        actr_quad_tree_query(_actr_ui_state->tree, &bounds, state->results);
        if (state->results->count == 0)
        {
            if (state->delete == 0)
            {
                struct MyPixel pix;
                pix.point = bounds.point;
                pix.size = bounds.size;
                // pix->color = actr_prng() * 0xffffffff;
                pix.color = state->color;

                actr_ui_container(pix.point.x, pix.point.y, pix.size.w, pix.size.h, state->color, state->color);
            }
        } else {
            if (state->delete) {
                struct ActrUIControl *control;
                for (int i = 0; i < state->results->count; i++)
                {
                    control = state->results->head[i];
                    if (control->type == ActrUITypeContainer)
                    {
                        actr_ui_remove_control(control->identity);
                    }
                }
            }
        }
        state->results->count = 0;
    }
}

[[clang::export_name("actr_step")]]
void actr_step(double delta)
{
    struct ActrQuadTreeBounds bounds;
    bounds.point.x = 0;
    bounds.point.y = 0;
    bounds.size.w = actrState->canvasSize.w;
    bounds.size.h = actrState->canvasSize.h;
    actr_ui_draw(delta);
}
