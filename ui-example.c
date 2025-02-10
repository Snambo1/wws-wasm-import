#include "actrwasm.h"
#include "actrquadtree.h"
#include "actrcanvas.h"
#include "actrprng.h"

struct ActrQuadTree *tree;
unsigned sequence = 1;
struct ActrVector *results;
struct ActrVector *list;
[[clang::export_name("actr_init")]]
void actr_init()
{
    // tree = actr_quad_tree_init(1, 256, 256, 256);
    // tree = actr_quad_tree_init(1, 256, 256, 32);
    // results = actr_vector_init(8, 8);
    list = actr_vector_init(8, 8);
}
extern void query_result(unsigned int value);

[[clang::export_name("actr_tap")]]
void actr_tap(int x, int y)
{
    return;
    /*
    struct ActrQuadTreeBounds area;

    area.x = x - 2;
    area.y = y - 2;
    area.size = 5;
    actr_quad_tree_query(tree, &area, results);
    for (int i = 0; i < results->count; i++)
    {
        query_result(*(unsigned int *)results->head[i]);
    }
    results->count = 0;
    x = actr_prng() * actrState->canvasSize.x;
    y = actr_prng() * actrState->canvasSize.y;
    int size = 2;
    int pad = size / 2;
    unsigned int *seq = actr_malloc(sizeof(unsigned int));
    *seq = sequence++;
    actr_quad_tree_insert(tree, actr_quad_tree_leaf(x - pad, y - pad, size, seq));*/
}

// [[clang::export_name("actr_move")]] void actr_move(int x, int y) { }
extern void removed();
float maxCount = 0;
[[clang::export_name("actr_step")]]
void actr_step()
{
    maxCount++;
    actr_canvas2d_fill_style(0, 0, 0, 100);
    actr_canvas2d_fill_rect(-10, -10, 9999, 9999);

    // if (maxCount < 1000)
    {
        for (int i = 0; i < 500; i++)
        {
            int x = actr_prng() * (actrState->canvasSize.x - 10) + 5;
            int y = actr_prng() * (actrState->canvasSize.y - 10) + 5;
            struct ActrQuadTreeBounds *area = actr_malloc(sizeof(struct ActrQuadTreeBounds));
            area->x = x;
            area->y = y;
            // area->x = 123;
            // area->y = 123;
            area->size = 5;
            actr_vector_add(list, area);
        }
    }

    actr_canvas2d_stroke_style(200, 200, 200, 100);

    for (int i = 0; i < list->count; i++)
    {
        _actr_quad_tree_draw_bounds(list->head[i]);
    }
    // if (maxCount < 900)
    {

        
        while (list->count)
        {
            //int index = actr_prng() * list->count;
            actr_free(list->head[0]);
            actr_vector_remove(list, 0);
        }
    }

    
    char *mem = actr_memory_report();
    actr_canvas2d_measure_text(mem);

    actr_canvas2d_fill_style(50, 50, 50, 100);
    actr_canvas2d_fill_rect(0, 10 - actrState->textSize.y, actrState->textSize.x, actrState->textSize.y);

    actr_canvas2d_fill_style(255, 255, 255, 100);
    actr_canvas2d_fill_text(0, actrState->textSize.y, mem);
    actr_free(mem);
    // actr_quad_tree_insert(tree, actr_quad_tree_leaf(x - pad, y - pad, size, 0));
    // actr_quad_tree_draw(tree);
}
