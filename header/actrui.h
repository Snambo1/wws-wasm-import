#ifndef ACTRUI_H
#define ACTRUI_H
#include "actrwasm.h"
#include "actrquadtree.h"
#include "actrcanvas.h"

struct ActrVector *_actr_ui_results;
struct ActrQuadTree *_actr_ui_tree;
int _actr_ui_sequence = 1;

int _actr_ui_hover = 0;

enum ActrUIType
{
    ActrUITypeButton
};

struct ActrUIControl {
    enum ActrUIType type;
    int identity;
    
};
struct ActrUIButton
{
    enum ActrUIType type;
    int identity;
    int x, y, w, h;
    const char *text;
    struct ActrQuadTreeLeaf *leaf;
};

void actr_ui_init()
{
    _actr_ui_results = actr_vector_init(8, 8);
    _actr_ui_tree = actr_quad_tree_init(1, 256, 256, 64);
}

extern void tap_identity(int identity);

void actr_ui_move(int x, int y)
{
    struct ActrQuadTreeBounds area;
    area.x = x;
    area.y = y;
    area.size = 1;
    actr_quad_tree_query(_actr_ui_tree, &area, _actr_ui_results);

    int identity = 0;
    for (int i = 0; i < _actr_ui_results->count; i++)
    {
        struct ActrUIControl *type = (struct ActrUIControl *)_actr_ui_results->head[i];
        if (type->identity > identity) {
            identity = type->identity;
        }
    }
    _actr_ui_hover = identity;

    _actr_ui_results->count = 0;

}

void actr_ui_tap(int x, int y)
{
    struct ActrQuadTreeBounds area;
    area.x = x;
    area.y = y;
    area.size = 1;
    actr_quad_tree_query(_actr_ui_tree, &area, _actr_ui_results);

    int identity = 0;
    for (int i = 0; i < _actr_ui_results->count; i++)
    {
        struct ActrUIControl *type = (struct ActrUIControl *)_actr_ui_results->head[i];
        if (type->identity > identity) {
            identity = type->identity;
        }
    }

    _actr_ui_results->count = 0;

}

int actr_ui_button(int x, int y, int w, int h, const char *text)
{
    struct ActrUIButton *button = actr_malloc(sizeof(struct ActrUIButton));
    button->type = ActrUITypeButton;
    button->identity = _actr_ui_sequence++;
    button->x = x;
    button->y = y;
    button->w = w;
    button->h = h;
    button->text = text;
    button->leaf = actr_quad_tree_leaf(x, y, w > h ? w : h, button);
    actr_quad_tree_insert(_actr_ui_tree, button->leaf);
    return button->identity;
}
extern void got_button();
void _actr_ui_draw_button(struct ActrUIButton *button)
{
    if (_actr_ui_hover == button->identity) {
    actr_canvas2d_fill_style(100, 200, 100, 100);

    }else{
    actr_canvas2d_fill_style(100, 100, 100, 100);

    }
    actr_canvas2d_fill_rect(button->x, button->y, button->w, button->h);

    actr_canvas2d_fill_style(0, 0, 0, 100);
    actr_canvas2d_fill_text(button->x + 5, button->y + button->h - 5, button->text);

    actr_canvas2d_stroke_style(200, 200, 200, 100);
    actr_canvas2d_stroke_rect(button->x, button->y, button->w, button->h);
}
void actr_ui_draw()
{
    // clear canvas
    actr_canvas2d_fill_style(0, 0, 0, 100);
    actr_canvas2d_fill_rect(-10, -10, actrState->canvasSize.x + 20, actrState->canvasSize.y + 20);
    struct ActrQuadTreeBounds area;
    area.x = 0;
    area.y = 0;
    area.size = actrState->canvasSize.x > actrState->canvasSize.y ? actrState->canvasSize.x : actrState->canvasSize.y;
    actr_quad_tree_query(_actr_ui_tree, &area, _actr_ui_results);

    for (int i = 0; i < _actr_ui_results->count; i++)
    {
        enum ActrUIType *type = (enum ActrUIType *)_actr_ui_results->head[i];
        switch (*type)
        {
        case ActrUITypeButton:
            _actr_ui_draw_button(_actr_ui_results->head[i]);
            break;
        }
    }

    _actr_ui_results->count = 0;

    // draw memory report
    char *mem = actr_memory_report();
    actr_canvas2d_measure_text(mem);
    actr_canvas2d_fill_style(0, 0, 0, 100);
    actr_canvas2d_fill_rect(0, 0, actrState->textSize.x + 1, actrState->textSize.y + 3);
    actr_canvas2d_fill_style(255, 255, 255, 100);
    actr_canvas2d_fill_text(0, actrState->textSize.y, mem);
    actr_free(mem);

    actr_quad_tree_draw(_actr_ui_tree);
}

#endif