#include "actrwasm.h"
#include "actrquadtree.h"
#include "actrcanvas.h"
#include "actrprng.h"

struct ActrQuadTree *tree;
int sequence = 1;
struct ActrVector *results;
extern void intersect(int i);
extern void no_intersect(int i);
extern void contain(int i);
extern void no_contain(int i);

[[clang::export_name("actr_init")]]
void actr_init()
{
    for (int i = 0; i < 5; i++)
    {
        struct ActrQuadTreeBounds one;
        one.x = 0;
        one.y = 0;
        one.size = 4;

        struct ActrQuadTreeBounds two;
        two.x = i;
        two.y = 0;
        two.size = 4;

        if (_actr_quad_tree_bounds_contains(&one, &two))
        {
            contain(i);
        }
        else
        {
            no_contain(i);
        }
    }

    tree = actr_quad_tree_init(1, 256, 256, 256);
    // tree = actr_quad_tree_init(1, 256, 256, 32);
    results = actr_vector_init(8, 8);
    // list = actr_vector_init(8, 8);
}
extern void query_result(int value);

//[[clang::export_name("actr_tap")]]
// void actr_tap(int x, int y)
[[clang::export_name("actr_move")]]
void actr_move(int x, int y)
{
    //*
    struct ActrQuadTreeBounds area;

    int pad = 2;
    int size = 5;
    area.x = x - pad;
    area.y = y - pad;
    area.size = size;
    actr_quad_tree_query(tree, &area, results);
    for (int i = 0; i < results->count; i++)
    {
        query_result(*(int *)results->head[i]);
    }
    if (results->count > 0)
    {
        results->count = 0;
        return;
    }
    // x = actr_prng() * actrState->canvasSize.x;
    // y = actr_prng() * actrState->canvasSize.y;
    int *seq = actr_malloc(sizeof(int));
    *seq = sequence++;
    actr_quad_tree_insert(tree, actr_quad_tree_leaf(x - pad, y - pad, size, seq));

    //*/
}

// [[clang::export_name("actr_move")]] void actr_move(int x, int y) { }
[[clang::export_name("actr_step")]]
void actr_step()
{
    actr_canvas2d_fill_style(0, 0, 0, 100);
    actr_canvas2d_fill_rect(-10, -10, actrState->canvasSize.x + 20, actrState->canvasSize.y + 20);

    actr_quad_tree_draw(tree);

    char *mem = actr_memory_report();
    actr_canvas2d_measure_text(mem);

    actr_canvas2d_fill_style(50, 50, 50, 100);
    actr_canvas2d_fill_rect(1, 1, actrState->textSize.x, actrState->textSize.y);

    actr_canvas2d_fill_style(255, 255, 255, 100);
    actr_canvas2d_fill_text(0, actrState->textSize.y, mem);
    actr_free(mem);

    for (int i = 0; i < 100; i++)
    {
        int x = actr_prng() * actrState->canvasSize.x;
        int y = actr_prng() * actrState->canvasSize.y;
        int size = 3 + actr_prng() * 9;

        struct ActrQuadTreeBounds area;
        area.x = x;
        area.y = y;
        area.size = size;
        actr_quad_tree_query(tree, &area, results);
        for (int i = 0; i < results->count; i++)
        {
            query_result(*(int *)results->head[i]);
        }
        if (results->count > 0)
        {
            results->count = 0;
            return;
        }

        actr_quad_tree_insert(tree, actr_quad_tree_leaf(x, y, size, 0));
    }
    // actr_quad_tree_draw(tree);
}
