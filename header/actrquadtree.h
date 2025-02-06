#ifndef ACTRQUADTREE_H
#define ACTRQUADTREE_H
#include "actralloc.h"
#include "actrvector.h"
#include "actrcanvas.h"

extern void _actr_debug_length(char *ptr, int len, int val);
void actr_debug(char *ptr, int val)
{
    _actr_debug_length(ptr, strlen(ptr), val);
}
struct ActrQuadTreeBounds
{
    long top;
    long right;
    long bottom;
    long left;
};
struct ActrQuadTreeLeaf
{
    struct ActrQuadTreeBounds *bounds;
    void *item;
};
struct ActrQuadTree
{
    int root;
    struct ActrQuadTreeBounds *bounds;
    struct ActrQuadTree *one;
    struct ActrQuadTree *two;
    struct ActrQuadTree *three;
    struct ActrQuadTree *four;
    struct ActrVector *items;
};

struct ActrQuadTreeBounds *_actr_quad_tree_bounds(long top, long right, long bottom, long left)
{
    struct ActrQuadTreeBounds *bounds = actr_malloc(sizeof(struct ActrQuadTreeBounds));
    bounds->top = top;
    bounds->right = right;
    bounds->bottom = bottom;
    bounds->left = left;
    return bounds;
}
struct ActrQuadTreeLeaf *_actr_quad_tree_leaf(long top, long right, long bottom, long left, void *item)
{
    struct ActrQuadTreeLeaf *leaf = actr_malloc(sizeof(struct ActrQuadTreeLeaf));
    leaf->bounds = _actr_quad_tree_bounds(top, right, bottom, left);
    leaf->item = item;
    return leaf;
}

// 1 2
// 4 3
struct ActrQuadTree *actr_quad_tree_init()
{
    struct ActrQuadTree *result = actr_malloc(sizeof(struct ActrQuadTree));
    result->root = 1;
    result->bounds = _actr_quad_tree_bounds(0, 64, 64, 0);
    return result;
}
int _actr_quad_tree_bounds_contains(struct ActrQuadTreeBounds *bounds, struct ActrQuadTreeBounds *other)
{

    if (other->top < bounds->top)
    {
        actr_debug("1contains one", other->top);
        actr_debug("2contains one", bounds->top);
        return 0;
    }
    // 566 > 0
    if (other->right > bounds->right)
    {
        actr_debug("1contains two", other->right);
        actr_debug("2contains two", bounds->right);
        return 0;
    }
    if (other->bottom > bounds->bottom)
    {
        actr_debug("1contains three", other->bottom);
        actr_debug("2contains three", bounds->bottom);
        return 0;
    }
    if (other->left < bounds->left)
    {
        actr_debug("1contains four", other->left);
        actr_debug("2contains four", bounds->left);
        return 0;
    }
    return 1;
}
// 1 2
// 4 3

void _actr_quad_tree_grow(struct ActrQuadTree *tree)
{
    long size = tree->bounds->right - tree->bounds->left;
    actr_debug("grow size", size);
    long grow = (size) / 2;
    actr_debug("grow grow", grow);
    struct ActrQuadTree *new;
    if (tree->one)
    {
        // becomes 1.3
        new = actr_quad_tree_init();
        new->root = 0;
        new->bounds->top = tree->bounds->top - grow;
        new->bounds->right = tree->one->bounds->right;
        new->bounds->bottom = tree->one->bounds->bottom;
        new->bounds->left = tree->bounds->left - grow;
        new->three = tree->one;
        tree->one = new;
    }
    if (tree->two)
    {
        // 1 2
        // 4 3
        // becomes 2.4
        new = actr_quad_tree_init();
        new->root = 0;
        // 2
        new->bounds->top = tree->bounds->top - grow;
        new->bounds->right = tree->bounds->right + grow;
        new->bounds->bottom = tree->two->bounds->bottom;
        new->bounds->left = tree->two->bounds->left;
        new->four = tree->two;
        tree->two = new;
    }
    if (tree->three)
    {
        // 1 2
        // 4 3
        // becomes 3.1
        new = actr_quad_tree_init();
        new->root = 0;
        // 3
        new->bounds->top = tree->three->bounds->top;
        new->bounds->right = tree->bounds->right + grow;
        new->bounds->bottom = tree->bounds->bottom + grow;
        new->bounds->left = tree->three->bounds->left;
        new->one = tree->three;
        tree->three = new;
    }
    if (tree->four)
    {
        // 1 2
        // 4 3
        // becomes 4.2
        new = actr_quad_tree_init();
        new->root = 0;
        // 4
        new->bounds->top = tree->four->bounds->top;
        new->bounds->right = tree->four->bounds->right;
        new->bounds->bottom = tree->bounds->bottom + grow;
        new->bounds->left = tree->bounds->left - grow;
        new->two = tree->four;
        tree->four = new;
    }
    tree->bounds->top -= grow;
    tree->bounds->right += grow;
    tree->bounds->bottom += grow;
    tree->bounds->left -= grow;
}
int _actr_quad_tree_index(struct ActrQuadTree *tree, struct ActrQuadTreeBounds *bounds)
{
    // 1 2
    // 4 3
    long ymid = tree->bounds->top + (tree->bounds->bottom - tree->bounds->top) / 2;
    long xmid = tree->bounds->left + (tree->bounds->right - tree->bounds->left) / 2;
    if (bounds->bottom < ymid)
    {
        if (bounds->right < xmid)
        {
            return 1;
        }
        return 2;
    }
    if (bounds->right < xmid)
    {
        return 4;
    }
    return 3;
}
void actr_quad_tree_draw(struct ActrQuadTree *tree)
{
    struct ActrQuadTreeLeaf *leaf;
    if (tree->items)
    {
        for (int i = 0; i < tree->items->pointer; i++)
        {
            leaf = *(struct ActrQuadTreeLeaf **)(tree->items->head + i * sizeof(void *));
            actr_canvas2d_stroke_rect(leaf->bounds->left - 0.5, leaf->bounds->top - 0.5, leaf->bounds->right - leaf->bounds->left, leaf->bounds->bottom - leaf->bounds->top);
        }
    }
}
void actr_quad_tree_insert(struct ActrQuadTree *tree, long top, long right, long bottom, long left, void *item)
{
    struct ActrQuadTreeLeaf *leaf = _actr_quad_tree_leaf(top, right, bottom, left, item);
    if (tree->root)
    {
        while (!_actr_quad_tree_bounds_contains(tree->bounds, leaf->bounds))
        {
            _actr_quad_tree_grow(tree);
        }
    }
    if (!tree->items)
    {
        tree->items = actr_malloc(sizeof(struct ActrVector));
        actr_vector_add(tree->items, leaf);
        return;
    } else if (tree->items) {
        actr_vector_add(tree->items, leaf); 
    }
    int index = _actr_quad_tree_index(tree, leaf->bounds);

    if (index == 1)
    {
    }
    else if (index == 2)
    {
    }
    else if (index == 3)
    {
    }
    else if (index == 4)
    {
    }
    actr_debug("qt insert index", index);
}
#endif
