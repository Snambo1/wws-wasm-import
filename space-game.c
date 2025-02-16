#include "actrwasm.h"
#include "actrcanvas.h"
#include "actrformat.h"
#include "actrhashtable.h"
#include "actrquadtree.h"
#include "actrprng.h"

struct Vector2D
{
    double x;
    double y;
};

struct MyShip
{
    struct Vector2D position;
    struct Vector2D velocity;
    double rotation;
};

struct MyState
{
    struct MyShip player;
    float aspect;
    char keys[256];
    struct ActrQuadTree *tree;
    struct ActrVector *result;
    struct ActrPoint lastGrid;
};

void loadArea(struct ActrPoint grid);

struct MyState *state;
double gridSize = 1024;

[[clang::export_name("actr_init")]]
void actr_init()
{
    state = actr_malloc(sizeof(struct MyState));
    state->player.position.x = actrState->canvasSize.w / 2.0f;
    state->player.position.y = actrState->canvasSize.h / 2.0f;
    // state->gridBucket = actr_hash_table_init();
    state->tree = actr_quad_tree_init(1, 0, 0, gridSize, 0);
    state->result = actr_vector_init(8, 8);
    struct ActrPoint grid;
    loadArea(grid);
}

void aspect(float a);
[[clang::export_name("actr_resize")]]
void actr_resize(float w, float h)
{
    state->aspect = w / h;
    aspect(state->aspect);
}

void gotkey(double key);
[[clang::export_name("actr_key_down")]]
void actr_key_down(int key)
{
    state->keys[key] = 1;
}

[[clang::export_name("actr_key_up")]]
void actr_key_up(int key)
{
    state->keys[key] = 0;
    gotkey((key));
}

double wrapN(double value, double N)
{
    if (value > N)
    {
        return value - N;
    }
    else if (value < -N)
    {
        return value + N;
    }
    return value;
}
double wrapPI(double value)
{
    return wrapN(value, PI);
}
double wrapTAU(double value)
{
    return wrapN(value, TAU);
}

double wrapPITAU(double value)
{
    if (value > PI)
    {
        return value - TAU;
    }
    else if (value < -PI)
    {
        return value + TAU;
    }
    return value;
}

void rotatePoint(struct Vector2D *point, double cos, double sin)
{
    // double x = actr_sin(rotation);
    // double y = actr_cos(rotation);
    double x = point->x * cos - point->y * sin;
    double y = point->x * sin + point->y * cos;
    point->x = x;
    point->y = -y;
}
void drawShip(struct MyShip *ship, int thrusting)
{
    struct Vector2D pt;

    double sin = actr_sin(ship->rotation);
    double cos = actr_cos(ship->rotation);
    float length = 25.0f;

    long long x = actrState->canvasSize.w / 2;
    long long y = actrState->canvasSize.h / 2;

    actr_canvas2d_stroke_style(255, 255, 255, 100);
    actr_canvas2d_begin_path();

    actr_canvas_moveto(x, y);

    // start at bottom left point
    pt.x = -0.5;
    pt.y = 0.5;
    rotatePoint(&pt, cos, sin);
    actr_canvas_lineto(x + pt.x * length, y + pt.y * length);

    // save starting point

    // line to front of ship
    pt.x = 0;
    pt.y = -0.5;
    rotatePoint(&pt, cos, sin);
    actr_canvas_lineto(x + pt.x * length, y + pt.y * length);

    // line to bottom right
    pt.x = 0.5;
    pt.y = 0.5;
    rotatePoint(&pt, cos, sin);
    actr_canvas_lineto(x + pt.x * length, y + pt.y * length);

    // close the path
    actr_canvas_lineto(x, y);
    actr_canvas2d_stroke();

    if (thrusting)
    {
        pt.x = 0;
        pt.y = 0.5;
        rotatePoint(&pt, cos, sin);
        actr_canvas2d_stroke_style(255, 0, 0, 100);
        actr_canvas2d_begin_path();
        actr_canvas_moveto(x, y);
        actr_canvas_lineto(x + pt.x * length, y + pt.y * length);
        actr_canvas2d_stroke();
    }
}

struct ActrPoint togrid(struct Vector2D point)
{
    struct ActrPoint result;
    result.x = (point.x < 0 ? (point.x - gridSize) : point.x) / gridSize;
    result.y = (point.y < 0 ? (point.y - gridSize) : point.y) / gridSize;
    return result;
}

struct ActrPoint fromgrid(struct ActrPoint point)
{
    struct ActrPoint result;
    result.x = point.x * gridSize;
    result.y = point.y * gridSize;
    return result;
}

/*struct ActrVector * getGridBucket(struct ActrPoint point)
{
    struct ActrHashTable * yTable = actr_hash_table_find(state->gridBucket, point.x);
    if (yTable == 0) {
        yTable = actr_hash_table_init();
        actr_hash_table_insert(state->gridBucket, point.x, yTable);
    }
    struct ActrVector * bucket = actr_hash_table_find(yTable, point.y);
    if (bucket == 0) {
        bucket = actr_vector_init(8, 8);
        actr_hash_table_insert(yTable, point.y, bucket);
    }
    return bucket;
}*/

void queryView()
{
    struct ActrQuadTreeBounds bounds;
    bounds.point.x = state->player.position.x - actrState->canvasSize.w / 2;
    bounds.point.y = state->player.position.y - actrState->canvasSize.h / 2;
    bounds.size.w = actrState->canvasSize.w;
    bounds.size.h = actrState->canvasSize.h;
    actr_quad_tree_query(state->tree, &bounds, state->result);
}

void drawView()
{
    long long x = state->player.position.x - actrState->canvasSize.w / 2;
    long long y = state->player.position.y - actrState->canvasSize.h / 2;

    queryView();
    actr_canvas2d_fill_style(255, 255, 255, 100);
    for (int i = 0; i < state->result->count; i++)
    {
        struct ActrQuadTreeLeaf *leaf = state->result->head[i];
        // actr_canvas2d_fill_text(leaf->bounds.point.x - x, leaf->bounds.point.y - y, leaf->item);
        actr_canvas2d_fill_rect(leaf->bounds.point.x - x, leaf->bounds.point.y - y, leaf->bounds.size.w, leaf->bounds.size.h);
    }
    state->result->count = 0;
}

void loadArea(struct ActrPoint grid)
{
    int index = 0;
    struct ActrPoint test;
    struct Vector2D point;
    struct ActrQuadTreeBounds bounds;
    struct ActrQuadTreeLeaf *leaf;
    bounds.size.h = gridSize;
    bounds.size.w = gridSize;

    for (long long y = grid.y - 1; y < grid.y + 2; y++)
    {
        for (long long x = grid.x - 1; x < grid.x + 2; x++)
        {
            test.x = x;
            test.y = y;
            bounds.point = fromgrid(test);

            actr_quad_tree_query(state->tree, &bounds, state->result);
            if (state->result->count == 0)
            {
                for (int i = 0; i < 99; i++)
                {
                    int w = 5 + actr_prng() * 25;
                    int h = 5 + actr_prng() * 25;

                    leaf = actr_quad_tree_leaf(
                        bounds.point.x + actr_prng() * gridSize,
                        bounds.point.y + actr_prng() * gridSize,
                        w,
                        h,
                        0
                    );
                    actr_quad_tree_insert(state->tree, leaf);
                }
            }
            else
            {
                state->result->count = 0;
            }
        }
    }
}

double frameAverage = 0.016;
[[clang::export_name("actr_step")]]
void actr_step(double delta)
{
    frameAverage = (frameAverage * 0.99) + (delta * 0.01);

    double y = actr_cos(state->player.rotation);
    double x = actr_sin(state->player.rotation);
    float length = 50;
    float force = 9999;
    state->player.rotation = wrapTAU(state->player.rotation);

    if (state->keys[1])
    {
        state->player.rotation += delta;
    }
    else if (state->keys[2])
    {
        state->player.rotation -= delta;
    }
    else if (state->keys[6])
    {
        double desired = actr_atan2(-state->player.velocity.y, state->player.velocity.x) - PI / 2.0;

        double rotationError = wrapPITAU(desired - state->player.rotation);

        if (rotationError > PI)
        {
            // rotationError -= PI;
        }
        else if (rotationError < -PI)
        {
            // rotationError += PI;
        }
        state->player.rotation += rotationError * delta;
    }

    state->player.position.x += state->player.velocity.x * delta;
    state->player.position.y += state->player.velocity.y * delta;

    int wrap = 0;
    if (wrap)
    {
        if (state->player.position.x > actrState->canvasSize.w)
        {
            state->player.position.x = 0;
        }
        else if (state->player.position.x < 0)
        {
            state->player.position.x = actrState->canvasSize.w;
        }

        if (state->player.position.y > actrState->canvasSize.h)
        {
            state->player.position.y = 0;
        }
        else if (state->player.position.y < 0)
        {
            state->player.position.y = actrState->canvasSize.h;
        }
    }

    int thrusting = 0;
    if (state->keys[5])
    {
        thrusting = 1;
        state->player.velocity.x += x * delta * force;
        state->player.velocity.y += y * delta * force;
    }

    state->player.velocity.x *= 0.9999;
    state->player.velocity.y *= 0.9999;

    actr_canvas2d_fill_style(0, 0, 0, 100);
    actr_canvas2d_fill_rect(-10, -10, actrState->canvasSize.w + 20, actrState->canvasSize.h + 20);

    // actr_canvas_moveto(state->player.position.x, state->player.position.y);

    // actr_canvas_lineto(state->player.position.x + x * length, state->player.position.y + y * length);
    drawView();
    drawShip(&state->player, thrusting);

    struct ActrFormatState *format = actr_format("pos %s x %s, grid %s x %s, velo %s x %s, fps %s, delta %s");
    //struct ActrFormatState *format = actr_format("pos %s");
    struct ActrPoint tgrid = togrid(state->player.position);

    if (tgrid.x != state->lastGrid.x || tgrid.y != state->lastGrid.y)
    {
        state->lastGrid = tgrid;
        loadArea(tgrid);
    }
    
    actr_format_float(format, state->player.position.x, 0);
    actr_format_float(format, state->player.position.y, 0);

    actr_format_float(format, tgrid.x, 0);
    actr_format_float(format, tgrid.y, 0);

    actr_format_float(format, state->player.velocity.x, 5);
    actr_format_float(format, state->player.velocity.y, 5);

    actr_format_float(format, frameAverage * 60 * 60, 5);
    actr_format_float(format, delta, 5);
    
    //actr_format_float(format, 0.0165, 5);


    char *text = actr_format_close(format);

    actr_canvas2d_fill_style(255, 255, 255, 100);
    actr_canvas2d_fill_text(5, actrState->canvasSize.h - 5, text);
    actr_free(text);

    actr_canvas2d_begin_path();
    actr_canvas_moveto(300,300);
    actr_canvas2d_arcTo(100, 100, 200, 200, 100);
    actr_canvas2d_stroke();
    text = actr_memory_report();
    actr_canvas2d_fill_text(5, actrState->canvasSize.h - 20, text);
    actr_free(text);
}