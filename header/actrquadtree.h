#ifndef ACTRQUADTREE_H
#define ACTRQUADTREE_H
#include "actralloc.h"
#include "actrvector.h"
#include "actrcanvas.h"

#define ACTR_QUAD_TREE_LIST_MAX 2

struct ActrQuadTreeBounds
{
    int x;
    int y;
    int size;
};

struct ActrQuadTreeLeaf
{
    struct ActrQuadTreeBounds bounds;
    void *item;
};
struct ActrQuadTreeLeaf *actr_quad_tree_leaf(int x, int y, int size, void *item)
{
    struct ActrQuadTreeLeaf *leaf = actr_malloc(sizeof(struct ActrQuadTreeLeaf));
    leaf->bounds.x = x;
    leaf->bounds.y = y;
    leaf->bounds.size = size;
    leaf->item = item;
    return leaf;
}

struct ActrQuadTree
{
    int root;
    struct ActrQuadTreeBounds bounds;
    struct ActrVector *items;
    struct ActrVector *stuck;
    struct ActrQuadTree **branch;
};
struct ActrQuadTree *actr_quad_tree_init(int root, int x, int y, int size)
{
    struct ActrQuadTree *result = actr_malloc(sizeof(struct ActrQuadTree));
    result->root = root;
    result->bounds.x = x;
    result->bounds.y = y;
    result->bounds.size = size;
    result->items = actr_vector_init(ACTR_QUAD_TREE_LIST_MAX, 0);
    result->stuck = actr_vector_init(4, 4);
    result->branch = actr_malloc(4 * sizeof(struct ActrQuadTree *));

    return result;
}
int _actr_quad_tree_bounds_intersects(struct ActrQuadTreeBounds *bounds, struct ActrQuadTreeBounds *other)
{
    /*
        RectA.Left < RectB.Right &&
        RectA.Right > RectB.Left &&
        RectA.Top > RectB.Bottom &&
        RectA.Bottom < RectB.Top
    */
    if (bounds->x >= other->x + other->size)
    {
        return 0;
    }

    if (other->x >= bounds->x + bounds->size)
    {
        return 0;
    }
    if (bounds->y >= other->y + other->size)
    {
        return 0;
    }
    if (other->y >= bounds->y + bounds->size)
    {
        return 0;
    }
    return 1;
}
int _actr_quad_tree_bounds_contains(struct ActrQuadTreeBounds *bounds, struct ActrQuadTreeBounds *other)
{
    if (bounds->x <= other->x &&
        bounds->x + bounds->size >= other->x + other->size &&
        bounds->y <= other->y &&
        bounds->y + bounds->size >= other->y + other->size)
    {
        return 1;
    }
    return 0;
}
void _actr_quad_tree_grow(struct ActrQuadTree *tree)
{
    struct ActrQuadTree *new;
    int size = tree->bounds.size;
    int halfSize = size / 2;
    if (tree->branch[0])
    {
        // 0 1
        // 3 2
        // 0 becomes 2 in new 0
        new = actr_quad_tree_init(0, tree->bounds.x - halfSize, tree->bounds.y - halfSize, size);
        new->branch[2] = tree->branch[0];
        tree->branch[0] = new;
    }
    if (tree->branch[1])
    {
        // 0 1
        // 3 2
        // 1 becomes 3 in new 1
        new = actr_quad_tree_init(0, tree->bounds.x + halfSize, tree->bounds.y - halfSize, size);
        new->branch[3] = tree->branch[1];
        tree->branch[1] = new;
    }
    if (tree->branch[2])
    {
        // 0 1
        // 3 2
        // 2 becomes 0
        new = actr_quad_tree_init(0, tree->bounds.x + halfSize, tree->bounds.y + halfSize, size);
        new->branch[0] = tree->branch[2];
        tree->branch[2] = new;
    }
    if (tree->branch[3])
    {
        // 0 1
        // 3 2
        // 3 becomes 1
        new = actr_quad_tree_init(0, tree->bounds.x - halfSize, tree->bounds.y + halfSize, size);
        new->branch[1] = tree->branch[3];
        tree->branch[3] = new;
    }

    tree->bounds.x -= halfSize;
    tree->bounds.y -= halfSize;
    tree->bounds.size += size;
}
int _actr_quad_tree_index(struct ActrQuadTree *tree, struct ActrQuadTreeBounds *bounds)
{
    // 0 1
    // 3 2
    int ymid = tree->bounds.y + (tree->bounds.size / 2);
    int xmid = tree->bounds.x + (tree->bounds.size / 2);
    
    if (bounds->y + bounds->size <= ymid)
    {
        // top half
        if (bounds->x + bounds->size <= xmid)
        {
            // left half
            return 0;
        }
        if (bounds->x >= xmid)
        {
            // right half
            return 1;
        }
    }
    else if (bounds->y >= ymid)
    {
        // bottom half
        if (bounds->x + bounds->size <= xmid)
        {
            // left half
            return 3;
        }
        if (bounds->x >= xmid)
        {
            // right half
            return 2;
        }
    }
    // qt_indx(result, tree->bounds.x, tree->bounds.y, tree->bounds.size, bounds.x, bounds.y, bounds.size);
    return -1;
}
void _actr_quad_tree_draw_bounds(struct ActrQuadTreeBounds *bounds)
{
    actr_canvas2d_stroke_rect(bounds->x, bounds->y, bounds->size, bounds->size);
}
void actr_quad_tree_draw(struct ActrQuadTree *tree)
{
    struct ActrQuadTreeLeaf *leaf;
    struct ActrVector *trees = actr_vector_init(20, 16);

    actr_vector_add(trees, tree);

    while (trees->count)
    {
        tree = trees->head[0];

        // actr_canvas2d_stroke_style(0, 255, 0, 50);
        // _actr_quad_tree_draw_bounds(&tree->bounds);

        if (tree->items)
        {
            actr_canvas2d_stroke_style(0, 255, 255, 50);
            for (int i = 0; i < tree->items->count; i++)
            {
                leaf = (struct ActrQuadTreeLeaf *)(tree->items->head[i]);
                _actr_quad_tree_draw_bounds(&leaf->bounds);
            }
        }

        if (tree->stuck)
        {
            actr_canvas2d_stroke_style(200, 200, 200, 50);
            for (int i = 0; i < tree->stuck->count; i++)
            {
                leaf = (struct ActrQuadTreeLeaf *)(tree->stuck->head[i]);
                _actr_quad_tree_draw_bounds(&leaf->bounds);
            }
        }

        if (tree->branch)
        {
            for (int i = 0; i < 4; i++)
            {
                if (tree->branch[i])
                {
                    actr_vector_add(trees, tree->branch[i]);
                }
            }
        }
        actr_vector_remove(trees, 0);
    }
    actr_vector_free(trees);
}
void actr_quad_tree_query(struct ActrQuadTree *root, struct ActrQuadTreeBounds *area, struct ActrVector *results)   
{
    struct ActrVector *list = actr_vector_init(16, 16);

    if (_actr_quad_tree_bounds_intersects(area, &root->bounds))
    {
        actr_vector_add(list, root);
    }

    while (list->count)
    {
        struct ActrQuadTree *tree = list->head[0];
        actr_vector_remove(list, 0);
        for (int i = 0; i < 4; i++)
        {
            if (tree->branch[i] && _actr_quad_tree_bounds_intersects(area, &tree->branch[i]->bounds))
            {
                actr_vector_add(list, tree->branch[i]);
            }
        }
        for (int i = 0; i < tree->items->count; i++)
        {
            struct ActrQuadTreeLeaf *leaf = tree->items->head[i];
            if (_actr_quad_tree_bounds_intersects(area, &leaf->bounds))
            {
                actr_vector_add(results, leaf->item);
            }
        }

        for (int i = 0; i < tree->stuck->count; i++)
        {
            struct ActrQuadTreeLeaf *leaf = tree->stuck->head[i];
            if (_actr_quad_tree_bounds_intersects(area, &leaf->bounds))
            {
                actr_vector_add(results, leaf->item);
            }
        }
    }
    actr_vector_free(list);
}
void actr_quad_tree_insert(struct ActrQuadTree *tree, struct ActrQuadTreeLeaf *newLeaf)
{
    if (tree->root)
    {
        while (!_actr_quad_tree_bounds_contains(&tree->bounds, &newLeaf->bounds))
        {
            _actr_quad_tree_grow(tree);
        }
    }
    
    actr_vector_add(tree->items, newLeaf);
    
    if (tree->items->count < ACTR_QUAD_TREE_LIST_MAX)
    {
        return;
    }
    for (int i = 0; i < tree->items->count; i++)
    {
        newLeaf = tree->items->head[i];

        int index = _actr_quad_tree_index(tree, &newLeaf->bounds);

        if (index < 0)
        {
            actr_vector_add(tree->stuck, newLeaf);
            continue;
        }

        if (!tree->branch[index])
        {
            int size = tree->bounds.size / 2;
            int x = tree->bounds.x;
            int y = tree->bounds.y;

            if (1 == index)
            {
                x += size;
            }
            else if (2 == index)
            {
                x += size;
                y += size;
            }
            else if (3 == index)
            {
                y += size;
            }

            tree->branch[index] = actr_quad_tree_init(0, x, y, size);
        }
        actr_quad_tree_insert(tree->branch[index], newLeaf);
    }
    tree->items->count = 0;
}
#endif
