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
    struct ActrVector *items;
    struct ActrVector *stuck;
    struct ActrQuadTree **branch;
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
struct ActrQuadTreeLeaf *actr_quad_tree_leaf(long top, long right, long bottom, long left, void *item)
{
    struct ActrQuadTreeLeaf *leaf = actr_malloc(sizeof(struct ActrQuadTreeLeaf));
    leaf->bounds = _actr_quad_tree_bounds(top, right, bottom, left);
    leaf->item = item;
    return leaf;
}

// 1 2
// 4 3
struct ActrQuadTree *actr_quad_tree_init(int root, int top, int right, int bottom, int left)
{

    struct ActrQuadTree *result = actr_malloc(sizeof(struct ActrQuadTree));
    result->root = root;
    result->bounds = _actr_quad_tree_bounds(top, right, bottom, left);
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
    if (tree->branch)
    {
        if (tree->branch[0])
        {
            // becomes 1.3
            new = actr_quad_tree_init(0, tree->bounds->top - grow, tree->branch[0]->bounds->right, tree->branch[0]->bounds->bottom, tree->bounds->left - grow);
            new->branch[2] = tree->branch[0];
            tree->branch[0] = new;
        }
        if (tree->branch[1])
        {
            // 1 2
            // 4 3
            // becomes 2.4
            new = actr_quad_tree_init(0, tree->bounds->top - grow, tree->bounds->right + grow, tree->branch[1]->bounds->bottom, tree->branch[1]->bounds->left);
            // 2
            new->branch[3] = tree->branch[1];
            tree->branch[1] = new;
        }
        if (tree->branch[2])
        {
            // 1 2
            // 4 3
            // becomes 3.1
            new = actr_quad_tree_init(0, tree->branch[2]->bounds->top, tree->bounds->right + grow, tree->bounds->bottom + grow, tree->branch[2]->bounds->left);
            // 3
            new->branch[0] = tree->branch[2];
            tree->branch[2] = new;
        }
        if (tree->branch[3])
        {
            // 1 2
            // 4 3
            // becomes 4.2
            new = actr_quad_tree_init(0, tree->branch[3]->bounds->top, tree->branch[3]->bounds->right, tree->bounds->bottom + grow, tree->bounds->left - grow);
            // 4
            new->branch[1] = tree->branch[3];
            tree->branch[3] = new;
        }
    }
    tree->bounds->top -= grow;
    tree->bounds->right += grow;
    tree->bounds->bottom += grow;
    tree->bounds->left -= grow;
    // actr_debug("qt grew width", tree->bounds->right - tree->bounds->left);
    // actr_debug("qt grew height", tree->bounds->bottom - tree->bounds->top);
}
int _actr_quad_tree_index(struct ActrQuadTree *tree, struct ActrQuadTreeBounds *bounds)
{
    // 1 2
    // 4 3
    long ymid = tree->bounds->top + (tree->bounds->bottom - tree->bounds->top) / 2;
    long xmid = tree->bounds->left + (tree->bounds->right - tree->bounds->left) / 2;

    actr_debug("top", tree->bounds->top);
    actr_debug("right", tree->bounds->right);
    actr_debug("bottom", tree->bounds->bottom);
    actr_debug("left", tree->bounds->left);
    actr_debug("xmid", xmid);
    actr_debug("ymid", ymid);

    if (bounds->bottom < ymid)
    {
        // top half
        if (bounds->right < xmid)
        {
            // left half
            return 0;
        }
        if (bounds->left >= xmid)
        {
            // right half
            return 1;
        }
    }
    else if (bounds->top >= ymid)
    {
        // bottom half
        if (bounds->right < xmid)
        {
            // left half
            return 3;
        }
        if (bounds->left >= xmid)
        {
            // right half
            return 2;
        }
    }
    return -1;
}

void _actr_quad_tree_draw_bounds(struct ActrQuadTreeBounds *bounds)
{
    actr_canvas2d_stroke_rect(bounds->left, bounds->top, bounds->right - bounds->left, bounds->bottom - bounds->top);
}
void actr_quad_tree_draw(struct ActrQuadTree *tree)
{
    struct ActrQuadTreeLeaf *leaf;
    if (tree->items)
    {
        actr_canvas2d_stroke_style(0, 255, 255, 50);
        for (int i = 0; i < tree->items->count; i++)
        {
            leaf = (struct ActrQuadTreeLeaf *)(tree->items->head[i]);
            _actr_quad_tree_draw_bounds(leaf->bounds);
            // actr_canvas2d_stroke_rect(leaf->bounds->left + 0.5, leaf->bounds->top + 0.5, leaf->bounds->right - leaf->bounds->left, leaf->bounds->bottom - leaf->bounds->top);
        }
    }
    if (tree->stuck) {
        actr_canvas2d_stroke_style(200, 200, 200, 50);
        for (int i = 0; i < tree->stuck->count; i++)
        {
            leaf = (struct ActrQuadTreeLeaf *)(tree->stuck->head[i]);
            _actr_quad_tree_draw_bounds(leaf->bounds);
            // actr_canvas2d_stroke_rect(leaf->bounds->left + 0.5, leaf->bounds->top + 0.5, leaf->bounds->right - leaf->bounds->left, leaf->bounds->bottom - leaf->bounds->top);
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (tree->branch[i])
        {
            actr_quad_tree_draw(tree->branch[i]);
        }
    }

    actr_canvas2d_stroke_style(0, 255, 0, 50);
    _actr_quad_tree_draw_bounds(tree->bounds);
}
void actr_quad_tree_insert(struct ActrQuadTree *tree, struct ActrQuadTreeLeaf *leaf)
{
    if (tree->root)
    {
        while (!_actr_quad_tree_bounds_contains(tree->bounds, leaf->bounds))
        {
            _actr_quad_tree_grow(tree);
        }
    }
    if (!tree->items)
    {
        tree->items = actr_vector_init(4, 0);
    }
    actr_vector_add(tree->items, leaf);
    actr_debug("vec add count", tree->items->count);
    if (tree->items->count < 4)
    {
        return;
    }
    for (int i = 0; i < 4; i++)
    {
        leaf = tree->items->head[i];
        // todo push items
        int index = _actr_quad_tree_index(tree, leaf->bounds);
        if (index < 0)
        {
            if (!tree->stuck)
            {
                tree->stuck = actr_vector_init(4, 4);
            }
            actr_vector_add(tree->stuck, leaf);
            continue;
        }
        if (!tree->branch)
        {
            tree->branch = actr_malloc(4 * sizeof(void *));
        }

        if (!tree->branch[index])
        {
            int width = (tree->bounds->right - tree->bounds->left) / 2;
            int height = (tree->bounds->bottom - tree->bounds->top) / 2;
            int top, right, bottom, left;
            if (index < 2)
            {
                top = tree->bounds->top;
                bottom = top + height;
            }
            else
            {
                bottom = tree->bounds->bottom;
                top = bottom - height;
            }
            if (index == 0 || index == 3)
            {
                left = tree->bounds->left;
                right = left + width;
            }
            else
            {
                right = tree->bounds->right;
                left = right - width;
            }

            tree->branch[index] = actr_quad_tree_init(0, top, right, bottom, left);
        }

        actr_quad_tree_insert(tree->branch[index], leaf);

        actr_debug("qt insert index", index);
    }
    tree->items->count = 0;
}
#endif
