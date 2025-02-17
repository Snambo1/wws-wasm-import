#include "actrformat.h"
#include "actrtime.h"
#include "actrui.h"
#include "actrprng.h"
#define PANEL_WIDTH 60

struct MyState
{
    int pointerDown;
    struct ActrVector *result;
    struct ActrVector *colorButtons;
    int deleteButton;
    unsigned int color;
    int delete;
    int pickerActive;
    struct ActrUIControlGradient *gradientAll;
    struct ActrUIControlGradient *gradientPick;
    struct ActrUIControlButton *colorRevert;
    struct ActrUIControlButton *colorAccept;
    struct ActrUIControlButton *colorCancel;
    struct ActrUIControlContainer *colorSample;
    unsigned int *colorCode;
    int colorCount;
    int nopaint;
    unsigned int revertColor;
    struct ActrUIControlButton *changeButton;
};

struct MyState *state;

void generateUI()
{
    struct ActrUIControlButton *button;

    for (int i = 0; i < state->colorButtons->count; i++)
    {
        button = state->colorButtons->head[i];
        actr_ui_remove_control(button);
    }
    state->colorButtons->count = 0;

    unsigned char r, g, b, a;
    int height = actrState->canvasSize.h / state->colorCount;
    for (int i = 0; i < state->colorCount; i++)
    {
        if (i == 0)
        {
            button = actr_ui_button(0, i * height, PANEL_WIDTH, height, "Erase");
            button->control.borderColor = actr_pack_bytes(255, 0, 0, 100);
            button->control.foregroundColor = actr_pack_bytes(255, 255, 255, 100);
        }
        else
        {
            button = actr_ui_button(0, i * height, PANEL_WIDTH, height, "");
            button->control.borderColor = actr_pack_bytes(0, 0, 0, 100);
            
        }

        button->control.backgroundColor = state->colorCode[i];
        button->control.backgroundColorHovered = state->colorCode[i];
        button->control.backgroundColorFocused = state->colorCode[i];

        button->control.foregroundColor = state->colorCode[i];
        button->control.foregroundColorHovered = state->colorCode[i];
        button->control.foregroundColorFocused = state->colorCode[i];

        button->control.borderColor = state->colorCode[i];
        button->control.borderColorHovered = state->colorCode[i];
        button->control.borderColorFocused = state->colorCode[i];

        button->control.state = (void *)state->colorCode[i];
        actr_vector_add(state->colorButtons, button);
    }
}
void removeColorPicker()
{
    actr_ui_remove_control(state->gradientAll);
    actr_ui_remove_control(state->gradientPick);
    actr_ui_remove_control(state->colorAccept);
    actr_ui_remove_control(state->colorCancel);
    actr_ui_remove_control(state->colorRevert);
    actr_ui_remove_control(state->colorSample);

    state->gradientAll = 0;
    state->gradientPick = 0;
    state->colorAccept = 0;
    state->colorCancel = 0;
    state->colorRevert = 0;
    state->colorSample = 0;

    state->pickerActive = 0;
    state->changeButton = 0;
}

void generateColorPicker(struct ActrUIControlButton *target)
{
    if (state->pickerActive)
    {
        removeColorPicker();
    }
    int height = 200;

    int top = target->control.leaf->bounds.point.y;

    if (top + height > actrState->canvasSize.h)
    {
        top -= (top + height - actrState->canvasSize.h + 2);
    }

    state->pickerActive = 1;
    state->gradientPick = actr_ui_gradient(PANEL_WIDTH + 1, top, height, height, target->control.backgroundColor, 0);
    state->gradientAll = actr_ui_gradient(PANEL_WIDTH + height + 2, top, height / 5, height, 0, 1);
    state->colorAccept = actr_ui_button(PANEL_WIDTH + height + height / 5 + 3, top, 70, 30, "Okay");
    state->colorCancel = actr_ui_button(PANEL_WIDTH + height + height / 5 + 3, top + 31, 70, 30, "Cancel");
    state->colorRevert = actr_ui_button(PANEL_WIDTH + height + height / 5 + 3, top + 62, 70, 30, "Revert");
    state->colorSample = actr_ui_container(PANEL_WIDTH + height + height / 5 + 3, top + 93, 70, 70);
    state->colorSample->control.backgroundColor = target->control.backgroundColor;
    state->revertColor = (unsigned int)target->control.state;
    state->changeButton = target;
}
[[clang::export_name("actr_init")]]
void actr_init()
{
    actr_ui_init();
    state = actr_malloc(sizeof(struct MyState));
    state->color = actr_pack_bytes(255, 255, 255, 100);
    state->result = actr_vector_init(4, 4);
    state->colorButtons = actr_vector_init(4, 4);

    unsigned int _colorCode[] = {
        actr_pack_bytes(0, 0, 0, 100),       // black
        actr_pack_bytes(255, 255, 255, 100), // white
        actr_pack_bytes(127, 127, 127, 100), // gray
        actr_pack_bytes(136, 0, 21, 100),    // dark red
        actr_pack_bytes(237, 28, 36, 100),   // red
        actr_pack_bytes(255, 127, 39, 100),  // orange
        actr_pack_bytes(255, 242, 0, 100),   // yellow
        actr_pack_bytes(34, 177, 76, 100),   // green
        actr_pack_bytes(0, 162, 232, 100),   // turq
        actr_pack_bytes(63, 72, 204, 100),   // indigo
        actr_pack_bytes(163, 73, 164, 100)   // purple
    };
    state->colorCount = sizeof(_colorCode) / sizeof(_colorCode[0]);
    state->colorCode = actr_malloc(sizeof(_colorCode[0]) * state->colorCount);
    for (int i = 0; i < state->colorCount; i++)
    {
        state->colorCode[i] = _colorCode[i];
    }

    generateUI();
    // actr_ui_gradient(200, 200, 200, 200, actr_pack_bytes(255, 0, 255, 100));
}

extern void picked(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
// actr_canvas2d_fill_gradient_all
[[clang::export_name("actr_pointer_double_tap")]]
void actr_pointer_double_tap(int x, int y)
{
    unsigned int color = actr_canvas2d_pick(x, y);
    struct ActrUIControl *tapped = actr_ui_tap(x, y);
    int index = actr_vector_find(state->colorButtons, tapped);
    if (index < 1)
    {
        return;
    }
    generateColorPicker((struct ActrUIControlButton *)tapped);
}
[[clang::export_name("actr_pointer_tap")]]
void actr_pointer_tap(int x, int y)
{
    struct ActrUIControl *tapped = actr_ui_tap(x, y);

    if ((struct ActrUIControlButton *)tapped == state->colorCancel)
    {
        removeColorPicker();
        return;
    }
    if ((struct ActrUIControlButton *)tapped == state->colorRevert)
    {
        state->changeButton->control.backgroundColor = state->revertColor;
        state->color = state->revertColor;
        removeColorPicker();
        return;
    }
    if ((struct ActrUIControlButton *)tapped == state->colorAccept)
    {
        state->changeButton->control.backgroundColor = state->colorSample->control.backgroundColor;
        state->color = state->colorSample->control.backgroundColor;
        removeColorPicker();
        return;
    } 

    if ((struct ActrUIControlGradient *)tapped == state->gradientAll)
    {
        unsigned int pick = actr_canvas2d_pick(x, y);
        state->gradientPick->control.backgroundColor = pick;
        state->colorSample->control.backgroundColor = pick;
        actr_ui_invalidate();
    }

    if ((struct ActrUIControlGradient *)tapped == state->gradientPick)
    {
        unsigned int pick = actr_canvas2d_pick(x, y);
        state->colorSample->control.backgroundColor = pick;
        actr_ui_invalidate();
    }

    int index = actr_vector_find(state->colorButtons, tapped);
    if (index < 0)
    {
        return;
    }
    struct ActrUIControlContainer *container = (struct ActrUIControlContainer *)tapped;
    state->color = container->control.backgroundColor;
    if (state->color == 100u)
    {
        state->delete = 1;
    }
    else
    {
        state->delete = 0;
    }
}

[[clang::export_name("actr_resize")]]
void actr_resize(int w, int h)
{
    actr_ui_invalidate();
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
    state->nopaint = 0;
}

void paint(int x, int y)
{
    if (state->pointerDown == 0 || actr_ui_state->hovered > 0)
    {
        return;
    }
    struct ActrQuadTreeBounds bounds;
    int size = 25;
    for (int i = 0; i < 10; i++)
    {
        bounds.point.x = x + actr_prng() * size - size / 2;
        bounds.point.y = y + actr_prng() * size - size / 2;
        bounds.size.w = 3 + actr_prng() * 10;
        bounds.size.h = 3 + actr_prng() * 10;

        if (bounds.point.x < PANEL_WIDTH)
        {
            bounds.point.x = PANEL_WIDTH;
        }
        else if (bounds.point.x + bounds.size.w >= actrState->canvasSize.w)
        {
            bounds.point.x -= ((bounds.point.x + bounds.size.w + 1) - actrState->canvasSize.w);
        }

        if (y < 0)
        {
            y = 0;
        }
        else if (bounds.point.y + bounds.size.h >= actrState->canvasSize.h)
        {
            bounds.point.y -= ((bounds.point.y + bounds.size.h + 1) - actrState->canvasSize.h);
        }

        // actr_quad_tree_query(state->tree, &bounds, state->results);
        actr_quad_tree_query(actr_ui_state->tree, &bounds, state->result);
        if (state->result->count == 0)
        {
            if (state->delete == 0)
            {
                struct ActrUIControlContainer *container = actr_ui_container(bounds.point.x, bounds.point.y, bounds.size.w, bounds.size.h);
                container->control.backgroundColor = state->color;
                container->control.foregroundColor = state->color;
                container->control.zindex = -1;
            }
        }
        else
        {
            if (state->delete)
            {
                struct ActrQuadTreeLeaf *leaf;
                struct ActrUIControl *control;
                for (int i = 0; i < state->result->count; i++)
                {
                    leaf = state->result->head[i];
                    control = leaf->item;
                    if (control->type == ActrUITypeContainer)
                    {
                        actr_ui_remove_control(control);
                    }
                }
            }
        }
        state->result->count = 0;
    }
}

[[clang::export_name("actr_pointer_move")]]
void actr_pointer_move(int x, int y)
{
    struct ActrUIControl *target = actr_ui_move(x, y);

    if (state->pickerActive && state->pointerDown)
    {
        if (state->gradientAll == (struct ActrUIControlGradient *)target)
        {
            state->nopaint = 1;
            unsigned int pick = actr_canvas2d_pick(x, y);
            ;
            state->gradientPick->control.backgroundColor = pick;
            state->colorSample->control.backgroundColor = pick;
            actr_ui_invalidate();
        }
        else if (state->gradientPick == (struct ActrUIControlGradient *)target)
        {
            state->nopaint = 1;
            unsigned int pick = actr_canvas2d_pick(x, y);
            ;
            state->colorSample->control.backgroundColor = pick;
            actr_ui_invalidate();
        }
    }
    if (state->nopaint == 0)
    {
        paint(x, y);
    }
}

[[clang::export_name("actr_step")]]
void actr_step(double delta)
{
    actr_ui_draw(delta);
}
