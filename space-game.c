#include "actrwasm.h"
#include "actrcanvas.h"
#include "actrformat.h"
#include "actrhashtable.h"

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
    struct ActrHashTable *gridBucket;
    struct ActrPoint lastGrid;
};

struct MyState *state;

[[clang::export_name("actr_init")]]
void actr_init()
{
    state = actr_malloc(sizeof(struct MyState));
    state->player.position.x = actrState->canvasSize.w / 2.0f;
    state->player.position.y = actrState->canvasSize.h / 2.0f;
    state->gridBucket = actr_hash_table_init();
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

    actr_canvas2d_stroke_style(255, 255, 255, 100);
    actr_canvas2d_begin_path();

    actr_canvas_moveto(ship->position.x, ship->position.y);

    // start at bottom left point
    pt.x = -0.5;
    pt.y = 0.5;
    rotatePoint(&pt, cos, sin);
    actr_canvas_lineto(ship->position.x + pt.x * length, ship->position.y + pt.y * length);

    // save starting point

    // line to front of ship
    pt.x = 0;
    pt.y = -0.5;
    rotatePoint(&pt, cos, sin);
    actr_canvas_lineto(ship->position.x + pt.x * length, ship->position.y + pt.y * length);

    // line to bottom right
    pt.x = 0.5;
    pt.y = 0.5;
    rotatePoint(&pt, cos, sin);
    actr_canvas_lineto(ship->position.x + pt.x * length, ship->position.y + pt.y * length);

    // close the path
    actr_canvas_lineto(ship->position.x, ship->position.y);
    actr_canvas2d_stroke();

    if (thrusting)
    {
        pt.x = 0;
        pt.y = 0.5;
        rotatePoint(&pt, cos, sin);
        actr_canvas2d_stroke_style(255, 0, 0, 100);
        actr_canvas2d_begin_path();
        actr_canvas_moveto(ship->position.x, ship->position.y);
        actr_canvas_lineto(ship->position.x + pt.x * length, ship->position.y + pt.y * length);
        actr_canvas2d_stroke();
    }
}
double gridSize = 1000;
struct ActrPoint togrid(struct Vector2D point)
{
    struct ActrPoint result;
    result.x = (point.x < 0 ? (point.x - gridSize) : point.x) / gridSize;
    result.y = (point.y < 0 ? (point.y - gridSize) : point.y) / gridSize;
    return result;
}

struct Vector2D fromgrid(struct ActrPoint point)
{
    struct Vector2D result;
    result.x = point.x * gridSize;
    result.y = point.y * gridSize;
    return result;
}

struct ActrVector * getGridBucket(struct ActrPoint point)
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
}
[[clang::export_name("actr_step")]]
void actr_step(double delta)
{
    double y = actr_cos(state->player.rotation);
    double x = actr_sin(state->player.rotation);
    float length = 50;
    float force = 99;
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

    state->player.velocity.x *= 0.99;
    state->player.velocity.y *= 0.99;

    actr_canvas2d_fill_style(0, 0, 0, 100);
    actr_canvas2d_fill_rect(-10, -10, actrState->canvasSize.w + 20, actrState->canvasSize.h + 20);

    // actr_canvas_moveto(state->player.position.x, state->player.position.y);

    // actr_canvas_lineto(state->player.position.x + x * length, state->player.position.y + y * length);

    drawShip(&state->player, thrusting);

    struct ActrFormatState *format = actr_format("grid %s x %s, velo %s x %s, ");
    struct ActrPoint tgrid = togrid(state->player.position);

    if (tgrid.x != state->lastGrid.x || tgrid.y != state->lastGrid.y)
    {
        state->lastGrid = tgrid;
    } 
    
    actr_format_float(format, tgrid.x, 0);
    actr_format_float(format, tgrid.y, 0);
    
    actr_format_float(format, state->player.velocity.x, 5);
    actr_format_float(format, state->player.velocity.y, 5   );
    
    char *text = actr_format_close(format);

    actr_canvas2d_fill_style(255, 255, 255, 100);
    actr_canvas2d_fill_text(5, actrState->canvasSize.h - 5, text);
    actr_free(text);
}