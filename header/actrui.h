#ifndef ACTRUI_H
#define ACTRUI_H
#include "actrwasm.h"
#include "actrquadtree.h"
#include "actrcanvas.h"

struct ActrUIState
{
    struct ActrVector *controls;
    struct ActrVector *results;
    struct ActrQuadTree *tree;
    int sequence;
    int hover;
    int focus;
};

enum ActrUIType
{
    ActrUITypeButton,
    ActrUITypeText
};

struct ActrUIControl
{
    enum ActrUIType type;
    int identity;
    struct ActrQuadTreeLeaf *leaf;
};
struct ActrUIControlButton
{
    enum ActrUIType type;
    int identity;
    struct ActrQuadTreeLeaf *leaf;
    char *label;
};
struct ActrUIControlText
{
    enum ActrUIType type;
    int identity;
    struct ActrQuadTreeLeaf *leaf;
    char *value;
    int cursor;
};

struct ActrUIState *_actr_ui_state;

void actr_ui_init()
{
    _actr_ui_state = actr_malloc(sizeof(struct ActrUIState));
    _actr_ui_state->controls = actr_vector_init(8, 8);
    _actr_ui_state->results = actr_vector_init(8, 8);
    _actr_ui_state->tree = actr_quad_tree_init(1, 256, 256, 64);
    _actr_ui_state->sequence = 1;
    _actr_ui_state->focus = 0;
    _actr_ui_state->hover = 0;
}
extern void uibutton(int key);
void actr_ui_key_down_text(struct ActrUIControlText *text, int key)
{
    uibutton(key);
    int newLength;
    char *newValue;
    int currentLength = strlen(text->value);
    if (key == 1)
    {
        // left arrow
        text->cursor--;
        if (text->cursor < 0)
        {
            text->cursor = 0;
        }
    }
    else if (key == 2)
    {
        // right arrow
        text->cursor++;
        if (text->cursor > currentLength)
        {
            text->cursor = currentLength;
        }
    }
    else if (key == 3)
    {
        // backspace
        if (text->cursor == 0)
        {
            return;
        }
        newLength = currentLength - 1;
        newValue = actr_malloc(newLength + 1);
        for (int i = 0; i < newLength; i++)
        {
            if (i < text->cursor - 1)
            {
                newValue[i] = text->value[i];
            }
            else
            {
                newValue[i] = text->value[i + 1];
            }
        }
        actr_free(text->value);
        text->value = newValue;
        text->cursor--;
    }
    else if (key == 4)
    {
        // delete
        if (text->cursor == currentLength)
        {
            return;
        }
        newLength = currentLength - 1;
        newValue = actr_malloc(newLength + 1);
        for (int i = 0; i < newLength; i++)
        {
            if (i <= text->cursor - 1)
            {
                newValue[i] = text->value[i];
            }
            else
            {
                newValue[i] = text->value[i + 1];
            }
        }
        actr_free(text->value);
        text->value = newValue;
    }
    else if (key >= 32 && key <= 126)
    {
        newLength = currentLength + 1;
        newValue = actr_malloc(newLength + 1);

        for (int i = 0; i < newLength; i++)
        {
            if (i < text->cursor)
            {
                newValue[i] = text->value[i];
            }
            else if (i == text->cursor)
            {
                newValue[i] = key;
            }
            else
            {
                newValue[i] = text->value[i - 1];
            }
        }
        actr_free(text->value);
        text->value = newValue;
        text->cursor++;
    }
}
int actr_ui_key_down(int key)
{

    // key
    // 1 left arrow
    // 2 right arrow
    // 3 backspace
    //  - 31 are available
    // when key >= 32 and key <= 126 normal ascii codes
    struct ActrUIControl *control;

    int focus = _actr_ui_state->focus;
    if (focus < 1)
    {
        return focus;
    }
    control = (struct ActrUIControl *)_actr_ui_state->controls->head[focus - 1];
    switch (control->type)
    {
    case ActrUITypeButton:
        break;
    case ActrUITypeText:
        actr_ui_key_down_text((struct ActrUIControlText *)control, key);
        break;
    }
    return focus;
}

void _actr_ui_query(int x, int y, int w, int h)
{
    struct ActrQuadTreeBounds area;
    area.x = x;
    area.y = y;
    area.w = w;
    area.h = h;
    actr_quad_tree_query(_actr_ui_state->tree, &area, _actr_ui_state->results);
}
void actr_ui_move(int x, int y)
{
    _actr_ui_query(x, y, 1, 1);

    int identity = -1;
    for (int i = 0; i < _actr_ui_state->results->count; i++)
    {
        struct ActrUIControl *type = (struct ActrUIControl *)_actr_ui_state->results->head[i];
        if (type->identity > identity)
        {
            identity = type->identity;
        }
    }
    _actr_ui_state->hover = identity;
    _actr_ui_state->results->count = 0;
}
void _actr_ui_set_focus(int identity)
{
    if (identity < 1)
    {
        _actr_ui_state->focus = 0;
        return;
    }
    if (_actr_ui_state->focus == identity)
    {
        return;
    }
    _actr_ui_state->focus = identity;
    struct ActrUIControl *control = (struct ActrUIControl *)_actr_ui_state->controls->head[identity - 1];
    if (control->type == ActrUITypeText)
    {
        struct ActrUIControlText *text = (struct ActrUIControlText *)control;
        text->cursor = strlen(text->value);
    }
}
int actr_ui_tap(int x, int y)
{
    _actr_ui_query(x, y, 1, 1);

    int identity = 0;
    for (int i = 0; i < _actr_ui_state->results->count; i++)
    {
        struct ActrUIControl *type = (struct ActrUIControl *)_actr_ui_state->results->head[i];
        if (type->identity > identity)
        {
            identity = type->identity;
        }
    }

    _actr_ui_state->results->count = 0;
    _actr_ui_set_focus(identity);
    return identity;
}

struct ActrUIControlButton *actr_ui_button(int x, int y, int w, int h, char *label)
{
    struct ActrUIControlButton *button = actr_malloc(sizeof(struct ActrUIControlButton));

    button->type = ActrUITypeButton;
    button->identity = _actr_ui_state->sequence++;
    button->leaf = actr_quad_tree_leaf(x, y, w, h, button);
    button->label = label;

    actr_quad_tree_insert(_actr_ui_state->tree, button->leaf);
    actr_vector_add(_actr_ui_state->controls, button);
    return button;
}

struct ActrUIControlText *actr_ui_text(int x, int y, int w, int h, char *value)
{
    struct ActrUIControlText *text = actr_malloc(sizeof(struct ActrUIControlText));

    text->type = ActrUITypeText;
    text->identity = _actr_ui_state->sequence++;
    text->leaf = actr_quad_tree_leaf(x, y, w, h, text);
    text->value = value;

    actr_quad_tree_insert(_actr_ui_state->tree, text->leaf);
    actr_vector_add(_actr_ui_state->controls, text);
    return text;
}

void _actr_ui_set_focus_style(int focused)
{
    if (focused)
    {
        actr_canvas2d_stroke_style(150, 250, 150, 100);
    }
    else
    {
        actr_canvas2d_stroke_style(200, 200, 200, 100);
    }
}
void _actr_ui_set_hover_style(int hovered)
{
    if (hovered)
    {
        actr_canvas2d_fill_style(150, 200, 150, 100);
    }
    else
    {
        actr_canvas2d_fill_style(150, 150, 150, 100);
    }
}

void _actr_ui_draw_text(struct ActrUIControlText *text)
{
    struct ActrQuadTreeBounds *bounds = &text->leaf->bounds;
    int hovered = _actr_ui_state->hover == text->identity;
    int focused = _actr_ui_state->focus == text->identity;

    _actr_ui_set_hover_style(hovered && !focused);

    actr_canvas2d_fill_rect(bounds->x, bounds->y, bounds->w, bounds->h);

    actr_canvas2d_fill_style(0, 0, 255, 100);
    int charWidth = 9;
    int padSide = 5;
    int maxChars = (bounds->w - padSide * 2) / charWidth;
    int halfChars = maxChars / 2;
    int charCount = strlen(text->value);
    int substart = text->cursor - halfChars;
    if (charCount > maxChars)
    {
        if (substart < 0)
        {
            substart = 0;
        }
        if (substart + maxChars > charCount)
        {
            substart -= substart + maxChars - charCount;
        }
        char *display = substr(text->value, substart, maxChars);
        actr_canvas2d_fill_text(bounds->x + padSide, bounds->y + bounds->h - 5, display);
        actr_free(display);
    }
    else
    {
        substart = 0;
        actr_canvas2d_fill_text(bounds->x + padSide, bounds->y + bounds->h - 5, text->value);
    }

    _actr_ui_set_focus_style(focused);
    actr_canvas2d_stroke_rect(bounds->x, bounds->y, bounds->w, bounds->h);

    if (focused)
    {
        actr_canvas2d_stroke_style(0, 0, 255, 100);
        actr_canvas2d_begin_path();

        int cursorStart = bounds->x + 5;
        actr_canvas_moveto(cursorStart + (text->cursor - substart) * charWidth, bounds->y + bounds->h - 3);
        actr_canvas_lineto(cursorStart + (text->cursor + 1 - substart) * charWidth, bounds->y + bounds->h - 3);
        actr_canvas2d_stroke();
    }
}

void _actr_ui_draw_button(struct ActrUIControlButton *button)
{
    struct ActrQuadTreeBounds *bounds = &button->leaf->bounds;

    int focused = _actr_ui_state->focus == button->identity;
    int hovered = _actr_ui_state->hover == button->identity;
    _actr_ui_set_hover_style(hovered && !focused);

    actr_canvas2d_fill_rect(bounds->x, bounds->y, bounds->w, bounds->h);

    actr_canvas2d_fill_style(0, 0, 0, 100);
    int charWidth = 9;
    int padSide = 5;
    int maxChars = (bounds->w - padSide * 2) / charWidth;
    int charCount = strlen(button->label);

    if (charCount > maxChars)
    {
        char *label = substr(button->label, 0, maxChars);
        actr_canvas2d_fill_text(bounds->x + padSide, bounds->y + bounds->h - 5, label);
        actr_free(label);
    }
    else
    {
        actr_canvas2d_fill_text(bounds->x + padSide, bounds->y + bounds->h - 5, button->label);
    }

    if (focused)
    {
        actr_canvas2d_stroke_style(200, 250, 200, 100);
    }
    else
    {
        actr_canvas2d_stroke_style(200, 200, 200, 100);
    }
    actr_canvas2d_stroke_rect(bounds->x, bounds->y, bounds->w, bounds->h);
}
void actr_ui_draw(double delta)
{
    // clear canvas
    actr_canvas2d_fill_style(0, 0, 0, 100);
    actr_canvas2d_fill_rect(-10, -10, actrState->canvasSize.w + 20, actrState->canvasSize.h + 20);

    _actr_ui_query(0, 0, actrState->canvasSize.w, actrState->canvasSize.h);

    for (int i = 0; i < _actr_ui_state->results->count; i++)
    {
        enum ActrUIType *type = (enum ActrUIType *)_actr_ui_state->results->head[i];
        switch (*type)
        {
        case ActrUITypeButton:
            _actr_ui_draw_button(_actr_ui_state->results->head[i]);
            break;
        case ActrUITypeText:
            _actr_ui_draw_text(_actr_ui_state->results->head[i]);
            break;
        }
    }

    _actr_ui_state->results->count = 0;

    // draw memory report
    char *mem = actr_memory_report();
    actr_canvas2d_measure_text(mem);
    actr_canvas2d_fill_style(0, 0, 0, 100);
    actr_canvas2d_fill_rect(0, 0, actrState->textSize.x + 1, actrState->textSize.y + 3);
    actr_canvas2d_fill_style(255, 255, 255, 100);
    actr_canvas2d_fill_text(0, actrState->textSize.y, mem);
    actr_free(mem);
}

#endif
