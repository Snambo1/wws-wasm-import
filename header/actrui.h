#ifndef ACTRUI_H
#define ACTRUI_H
#include "actrwasm.h"
#include "actrquadtree.h"
#include "actrcanvas.h"
#include "actrhashtable.h"
#include "actrmergesortmutate.h"

enum ActrUIType
{
    ActrUITypeButton,
    ActrUITypeText,
    ActrUITypeContainer,
    ActrUITypeGradient
};

struct ActrUIControl
{
    enum ActrUIType type;
    int identity;
    struct ActrQuadTreeLeaf *leaf;
    int hidden;
    struct ActrUIControlContainer *container;
    unsigned int foregroundColor;
    unsigned int backgroundColor;
    unsigned int borderColor;
    void * state;
    int zindex;
};

struct ActrUIState
{
    struct ActrHashTable *controls;
    struct ActrVector *results;
    struct ActrQuadTree *tree;
    int sequence;
    struct ActrUIControl * hovered;
    struct ActrUIControl * focused;
    int valid;
};

struct ActrUIControlGradient
{
    struct ActrUIControl control;
    int all;
};

struct ActrUIControlButton
{
    struct ActrUIControl control;
    char *label;
};
struct ActrUIControlText
{
    struct ActrUIControl control;
    char *value;
    int cursor;
};
struct ActrUIControlContainer
{
    struct ActrUIControl control;
};

struct ActrUIState * actr_ui_state;

void actr_ui_invalidate() {
    actr_ui_state->valid = 0;
}

void _actr_ui_control_dispose(struct ActrUIControl *control)
{
    actr_free(control->leaf);
    actr_free(control);
}

void _actr_ui_button_dispose(struct ActrUIControlButton *button)
{
    actr_free(button->label);
    _actr_ui_control_dispose((struct ActrUIControl *)button);
}

void _actr_ui_text_dispose(struct ActrUIControlText *text)
{
    actr_free(text->value);
    _actr_ui_control_dispose((struct ActrUIControl *)text);
}

void _actr_ui_container_dispose(struct ActrUIControlContainer *container)
{
    _actr_ui_control_dispose((struct ActrUIControl *)container);
}

void _actr_ui_gradient_dispose(struct ActrUIControlGradient *gradient)
{
    _actr_ui_control_dispose((struct ActrUIControl *)gradient);
}
void actr_ui_init()
{
    actr_ui_state = actr_malloc(sizeof(struct ActrUIState));
    actr_ui_state->controls = actr_hash_table_init();
    actr_ui_state->results = actr_vector_init(8, 8);
    actr_ui_state->tree = actr_quad_tree_init(1, 256, 256, 64, 0);
    actr_ui_state->sequence = 1;
    actr_ui_state->focused = (struct ActrUIControl *)0;
    actr_ui_state->hovered = (struct ActrUIControl *)0;
    actr_ui_invalidate();
}

struct ActrUIControl *actr_ui_get_control(int identity)
{
    return actr_hash_table_find(actr_ui_state->controls, identity);
}

void actr_ui_remove_control(void *remove)
{
    if (remove == 0) {
        return;
    }
    actr_ui_invalidate();
    struct ActrUIControl * control = remove;
    actr_quad_tree_remove(control->leaf);
    switch (control->type)
    {
    case ActrUITypeButton:
        _actr_ui_button_dispose((struct ActrUIControlButton *)control);
        break;
    case ActrUITypeText:
        _actr_ui_text_dispose((struct ActrUIControlText *)control);
        break;
    case ActrUITypeContainer:
        _actr_ui_container_dispose((struct ActrUIControlContainer *)control);
        break;
    case ActrUITypeGradient:
        _actr_ui_gradient_dispose((struct ActrUIControlGradient *)control);
        break;
    }
    actr_hash_table_delete(actr_ui_state->controls, control->identity);
}
void actr_ui_key_down_text(struct ActrUIControlText *text, int key)
{
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
        } else {
            actr_ui_invalidate();
        }
    }
    else if (key == 2)
    {
        // right arrow
        text->cursor++;
        if (text->cursor > currentLength)
        {
            text->cursor = currentLength;
        } else {
            actr_ui_invalidate();
        }
    }
    else if (key == 3)
    {
        // backspace
        if (text->cursor == 0)
        {
            return;
        }
        actr_ui_invalidate();
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
        actr_ui_invalidate();
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
        actr_ui_invalidate();
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
struct ActrUIControl * actr_ui_key_down(int key)
{

    // key
    // 1 left arrow
    // 2 right arrow
    // 3 backspace
    //  - 31 are available
    // when key >= 32 and key <= 126 normal ascii codes
    struct ActrUIControl *control = (struct ActrUIControl *)actr_ui_state->focused;

    if (control == 0)
    {
        return 0;
    }

    switch (control->type)
    {
    case ActrUITypeButton:
        break;
    case ActrUITypeText:
        actr_ui_key_down_text((struct ActrUIControlText *)control, key);
        break;
    case ActrUITypeContainer:
        break;
    case ActrUITypeGradient:
        break;
    }
    return control;
}
int _actr_ui_query_sort_comparator(void * a, void * b) {
    // return 0xffffffff - ((struct ActrUIControl *)control)->identity;
    struct ActrUIControl * c1 = (struct ActrUIControl *)a;
    struct ActrUIControl * c2 = (struct ActrUIControl *)b;
    if (c1->zindex == c2->zindex) {
        return c1->identity < c2->identity;    
    }
    return c1->zindex < c2->zindex;
}
void _actr_ui_query(int x, int y, int w, int h)
{
    struct ActrQuadTreeBounds area;
    area.point.x = x;
    area.point.y = y;
    area.size.w = w;
    area.size.h = h;
    actr_quad_tree_query(actr_ui_state->tree, &area, actr_ui_state->results);
    actr_merge_sort_mutate(actr_ui_state->results, 0, actr_ui_state->results->count - 1, _actr_ui_query_sort_comparator, 0);
}
struct ActrUIControl *actr_ui_move(int x, int y)
{
    _actr_ui_query(x, y, 1, 1);

    struct ActrUIControl * hovered = 0;
    for (int i = 0; i < actr_ui_state->results->count; i++)
    {
        struct ActrUIControl *result = (struct ActrUIControl *)actr_ui_state->results->head[i];
        if (hovered == 0 || result->zindex > hovered->zindex || (result->zindex == hovered->zindex && result->identity > hovered->identity))
        {
            hovered = result;
        }
    }
    actr_ui_state->results->count = 0;
    if (actr_ui_state->hovered == hovered)
    {
        return actr_ui_state->hovered;
    }
    actr_ui_invalidate();
    actr_ui_state->hovered = hovered;
    return actr_ui_state->hovered;
}
void _actr_ui_set_focus(struct ActrUIControl * control)
{
    if (actr_ui_state->focused == control)
    {
        return;
    }
    actr_ui_invalidate();
    actr_ui_state->focused = control;
    if (control->type == ActrUITypeText)
    {
        struct ActrUIControlText *text = (struct ActrUIControlText *)control;
        text->cursor = strlen(text->value);
    }
}
struct ActrUIControl * actr_ui_tap(int x, int y)
{
    _actr_ui_query(x, y, 1, 1);

    struct ActrUIControl * target = 0;
    for (int i = 0; i < actr_ui_state->results->count; i++)
    {
        struct ActrUIControl *result = (struct ActrUIControl *)actr_ui_state->results->head[i];
        if (target == 0 || result->identity > target->identity)
        {
            target = result;
        }
    }
    actr_ui_state->results->count = 0;
    _actr_ui_set_focus(target);
    return target;
}

struct ActrUIControlButton *actr_ui_button(int x, int y, int w, int h, char *label)
{
    struct ActrUIControlButton *button = actr_malloc(sizeof(struct ActrUIControlButton));

    button->control.type = ActrUITypeButton;
    button->control.identity = actr_ui_state->sequence++;
    button->control.leaf = actr_quad_tree_leaf(x, y, w, h, button);
    button->label = actr_heap_string(label);
    button->control.foregroundColor = actr_pack_bytes(255, 255, 255, 100);
    button->control.backgroundColor = actr_pack_bytes(127, 127, 127, 100);

    actr_quad_tree_insert(actr_ui_state->tree, button->control.leaf);
    actr_hash_table_insert(actr_ui_state->controls, button->control.identity, button);
    actr_ui_invalidate();
    return button;
}

struct ActrUIControlGradient *actr_ui_gradient(int x, int y, int w, int h, unsigned int color, int all)
{
    struct ActrUIControlGradient *gradient = actr_malloc(sizeof(struct ActrUIControlGradient));
    gradient->control.type = ActrUITypeGradient;
    gradient->control.identity = actr_ui_state->sequence++;
    gradient->control.leaf = actr_quad_tree_leaf(x, y, w, h, gradient);
    gradient->control.backgroundColor = color;
    gradient->all = all;

    actr_quad_tree_insert(actr_ui_state->tree, gradient->control.leaf);
    actr_hash_table_insert(actr_ui_state->controls, gradient->control.identity, gradient);
    actr_ui_invalidate();
    return gradient;
}
struct ActrUIControlContainer *actr_ui_container(int x, int y, int w, int h)
{
    struct ActrUIControlContainer *container = actr_malloc(sizeof(struct ActrUIControlContainer));
    container->control.type = ActrUITypeContainer;
    container->control.identity = actr_ui_state->sequence++;
    container->control.leaf = actr_quad_tree_leaf(x, y, w, h, container);

    actr_quad_tree_insert(actr_ui_state->tree, container->control.leaf);
    actr_hash_table_insert(actr_ui_state->controls, container->control.identity, container);
    actr_ui_invalidate();
    return container;
}


struct ActrUIControlText *actr_ui_text(int x, int y, int w, int h, char *value)
{
    struct ActrUIControlText *text = actr_malloc(sizeof(struct ActrUIControlText));

    text->control.type = ActrUITypeText;
    text->control.identity = actr_ui_state->sequence++;
    text->control.leaf = actr_quad_tree_leaf(x, y, w, h, text);
    text->value = actr_heap_string(value);

    actr_quad_tree_insert(actr_ui_state->tree, text->control.leaf);
    actr_hash_table_insert(actr_ui_state->controls, text->control.identity, text);
    actr_ui_invalidate();
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
struct ActrPoint *_actr_ui_get_control_position(struct ActrUIControl *control)
{
    struct ActrPoint *result = actr_malloc(sizeof(struct ActrPoint));

    result->x = control->leaf->bounds.point.x;
    result->y = control->leaf->bounds.point.y;

    control = (struct ActrUIControl *)control->container;

    while (control)
    {
        result->x += control->leaf->bounds.point.x;
        result->y += control->leaf->bounds.point.y;
        control = (struct ActrUIControl *)control->container;
    }
    return result;
}
int _actr_ui_control_is_hovered(void * control) {
    if (actr_ui_state->hovered == control)
    {
        return 1;
    }
    return 0;
}
int _actr_ui__control_is_focused(void * control) {
    if (actr_ui_state->hovered == control)
    {
        return 1;
    }
    return 0;
}
void _actr_ui_draw_text(struct ActrUIControlText *text)
{
    struct ActrPoint *position;
    struct ActrSize *size = &text->control.leaf->bounds.size;
    if (text->control.container)
    {
        position = _actr_ui_get_control_position((struct ActrUIControl *)text);
    }
    else
    {
        position = &text->control.leaf->bounds.point;
    }
    
    int hovered = _actr_ui_control_is_hovered(text);
    int focused = _actr_ui__control_is_focused(text);

    _actr_ui_set_hover_style(hovered == 1 && focused == 0);

    actr_canvas2d_fill_rect(position->x, position->y, size->w, size->h);

    actr_canvas2d_fill_style(0, 0, 255, 100);
    int charWidth = 9;
    int padSide = 5;
    int maxChars = (size->w - padSide * 2) / charWidth;
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
        actr_canvas2d_fill_text(position->x + padSide, position->y + size->h - 5, display);
        actr_free(display);
    }
    else
    {
        substart = 0;
        actr_canvas2d_fill_text(position->x + padSide, position->y + size->h - 5, text->value);
    }

    _actr_ui_set_focus_style(focused);
    actr_canvas2d_stroke_rect(position->x, position->y, size->w, size->h);

    if (focused)
    {
        actr_canvas2d_stroke_style(0, 0, 255, 100);
        actr_canvas2d_begin_path();

        int cursorStart = position->x + 5;
        actr_canvas_moveto(cursorStart + (text->cursor - substart) * charWidth, position->y + size->h - 3);
        actr_canvas_lineto(cursorStart + (text->cursor + 1 - substart) * charWidth, position->y + size->h - 3);
        actr_canvas2d_stroke();
    }

    if (text->control.container)
    {
        actr_free(position);
    }
}
void _actr_ui_draw_gradient(struct ActrUIControlGradient *gradient)
{
    struct ActrQuadTreeBounds *bounds = &gradient->control.leaf->bounds;
    if (gradient->all == 1)
    {
        actr_canvas2d_fill_gradient_all(bounds->point.x, bounds->point.y, bounds->size.w, bounds->size.h);
    }
    else
    {
        unsigned char r, g, b, a;
        actr_unpack_bytes(gradient->control.backgroundColor, &r, &g, &b, &a);
        actr_canvas2d_fill_gradient_pick(
            bounds->point.x, bounds->point.y, bounds->size.w, bounds->size.h,
            r, g, b
        );
    }
}
void _actr_ui_draw_container(struct ActrUIControlContainer *container)
{
    unsigned char r, g, b, a;

    actr_unpack_bytes(container->control.backgroundColor, &r, &g, &b, &a);
    actr_canvas2d_fill_style(r, g, b, a);
    actr_canvas2d_fill_rect(container->control.leaf->bounds.point.x, container->control.leaf->bounds.point.y, container->control.leaf->bounds.size.w, container->control.leaf->bounds.size.h);

    actr_unpack_bytes(container->control.foregroundColor, &r, &g, &b, &a);
    actr_canvas2d_stroke_style(r, g, b, a);
    actr_canvas2d_stroke_rect(container->control.leaf->bounds.point.x, container->control.leaf->bounds.point.y, container->control.leaf->bounds.size.w, container->control.leaf->bounds.size.h);
}
void _actr_ui_draw_button(struct ActrUIControlButton *button)
{
    struct ActrQuadTreeBounds *bounds = &button->control.leaf->bounds;
    unsigned char r, g, b, a;

    int focused = _actr_ui__control_is_focused(button);
    int hovered = _actr_ui_control_is_hovered(button);

    actr_unpack_bytes(button->control.backgroundColor, &r, &g, &b, &a);
    actr_canvas2d_fill_style(r, g, b, a);
    actr_canvas2d_fill_rect(bounds->point.x, bounds->point.y, bounds->size.w, bounds->size.h);

    actr_unpack_bytes(button->control.foregroundColor, &r, &g, &b, &a);
    actr_canvas2d_fill_style(r, g, b, a);

    int charWidth = 9;
    int padSide = 5;
    int maxChars = (bounds->size.w - padSide * 2) / charWidth;
    int charCount = strlen(button->label);

    if (charCount > maxChars)
    {
        char *label = substr(button->label, 0, maxChars);
        actr_canvas2d_fill_text(bounds->point.x + padSide, bounds->point.y + bounds->size.h - 5, label);
        actr_free(label);
    }
    else
    {
        actr_canvas2d_fill_text(bounds->point.x + padSide, bounds->point.y + bounds->size.h - 5, button->label);
    }

    if (focused)
    {
        actr_unpack_bytes(button->control.borderColor, &r, &g, &b, &a);
        actr_canvas2d_stroke_style(r, g, b, a);
    }
    else
    {
        actr_unpack_bytes(button->control.borderColor, &r, &g, &b, &a);
        actr_canvas2d_stroke_style(r, g, b, a);
    }
    actr_canvas2d_stroke_rect(bounds->point.x, bounds->point.y, bounds->size.w, bounds->size.h);
}
void actr_ui_draw(double delta)
{
    if (actr_ui_state->valid == 1) {
        return;
    }
    actr_ui_state->valid = 1;
    // clear canvas
    actr_canvas2d_fill_style(0, 0, 0, 100);
    actr_canvas2d_fill_rect(-10, -10, actrState->canvasSize.w + 20, actrState->canvasSize.h + 20);

    _actr_ui_query(0, 0, actrState->canvasSize.w, actrState->canvasSize.h);
    
    for (int i = 0; i < actr_ui_state->results->count; i++)
    {
        struct ActrUIControl *control = (struct ActrUIControl *)actr_ui_state->results->head[i];
        if (control->hidden)
        {
            continue;
        }
        int hidden = 0;
        struct ActrUIControlContainer *container = control->container;
        while (container)
        {
            if (container->control.hidden)
            {
                hidden = 1;
                break;
            }
            container = container->control.container;
        }
        if (hidden)
        {
            continue;
        }
        switch (control->type)
        {
        case ActrUITypeButton:
            _actr_ui_draw_button(actr_ui_state->results->head[i]);
            break;
        case ActrUITypeText:
            _actr_ui_draw_text(actr_ui_state->results->head[i]);
            break;
        case ActrUITypeContainer:
            _actr_ui_draw_container(actr_ui_state->results->head[i]);
            break;
        case ActrUITypeGradient:
            _actr_ui_draw_gradient(actr_ui_state->results->head[i]);
            break;
        }
    }

    actr_ui_state->results->count = 0;

    if (actrState->debug)
    {
        // draw memory report
        char *mem = actr_memory_report();
        actr_canvas2d_measure_text(mem);
        actr_canvas2d_fill_style(0, 0, 0, 100);

        actr_canvas2d_fill_rect(0, 0, actrState->textSize.x + 2, actrState->textSize.y + 3);
        actr_canvas2d_fill_style(255, 255, 255, 100);
        actr_canvas2d_fill_text(1, actrState->textSize.y, mem);
        actr_free(mem);

        actr_quad_tree_draw(actr_ui_state->tree);
    }
}

#endif
