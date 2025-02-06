#include "actrwasm.h"
#include "actrquadtree.h"
#include "actrcanvas.h"
#include "actrprng.h"

struct ActrQuadTree *tree;
char *name;
int tapCount = 0;
[[clang::export_name("actr_init")]]
void actr_init()
{
    actr_sprng(actr_time());
    tree = actr_quad_tree_init(1, 256, 512, 512, 256);
    for (int i = 0; i < 1000; i++)
    {
        actr_prngf();
    }
}
[[clang::export_name("actr_tap")]]
void actr_tap(long x, long y)
{
    int *tap = actr_malloc(sizeof(int));
    *tap = tapCount++;
    int pad = 4;
    actr_quad_tree_insert(tree, actr_quad_tree_leaf(y - pad, x + pad, y + pad, x - pad, tap));
    // actr_quad_tree_insert(tree, 0, 20, 20, 0, tap);
}

int top = 0;
void printLine(char *line)
{
    top += 15;
    actr_canvas2d_fill_text(10, top, (char *)line);
}
[[clang::export_name("actr_step")]]
void actr_step()
{
    top = 0;
    actr_canvas2d_fill_style(0, 0, 0, 100);
    actr_canvas2d_fill_rect(-10, -10, 9999, 9999);
    actr_canvas2d_stroke_style(255, 255, 255, 100);
    actr_quad_tree_draw(tree);
}
