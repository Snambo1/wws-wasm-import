#include "actrwasm.h"
#include "actrcanvas.h"
#include "actrformat.h"
#include "actrhashtable.h"
#include "actrquadtree.h"
#include "actrprng.h"
#include "spacegame.h"

struct MyState *state;

void draw_asteroid(struct MyAsteroid *asteroid)
{

    struct ActrPointD pt;
    struct ActrPointD start;
    actr_canvas2d_stroke_style(255, 255, 255, 100);

    asteroid->object.rotation += 0.0001;

    int x = asteroid->object.position.x - state->player.object.position.x + actrState->canvasSize.w / 2;
    int y = asteroid->object.position.y - state->player.object.position.y + actrState->canvasSize.h / 2;

    /*
    drawass(
        x, y,
        state->player.object.position.x, state->player.object.position.y,
        asteroid->object.position.x, asteroid->object.position.y,
        actrState->canvasSize.w / 2,
        actrState->canvasSize.h / 2
    );
    //*/

    // asteroid->object.mass = 10;
    actr_canvas2d_fill_style(255, 255, 255, 100);
    //*
    struct ActrFormatState *format = actr_format("%s.%s %sx%s");
    actr_format_int(format, asteroid->object.position.x);
    actr_format_int(format, asteroid->object.position.y);
    actr_format_str(format, OreNames[asteroid->ore]);
    actr_format_int(format, asteroid->object.mass);
    char *text = actr_format_close(format);
    actr_canvas2d_fill_text(x, y, text);
    actr_free(text);
    //*/
    actr_canvas2d_begin_path();
    pt.x = -0.70710676237;
    pt.y = -0.70710676237;
    rotate_point(&pt, asteroid->object.cos, asteroid->object.sin);
    start = pt;
    actr_canvas2d_moveto(x + pt.x * asteroid->object.distance, y + pt.y * asteroid->object.distance);

    pt.x = 0.70710676237;
    pt.y = -0.70710676237;
    rotate_point(&pt, asteroid->object.cos, asteroid->object.sin);
    actr_canvas2d_lineto(x + pt.x * asteroid->object.distance, y + pt.y * asteroid->object.distance);

    pt.x = 0.70710676237;
    pt.y = 0.70710676237;
    rotate_point(&pt, asteroid->object.cos, asteroid->object.sin);
    actr_canvas2d_lineto(x + pt.x * asteroid->object.distance, y + pt.y * asteroid->object.distance);

    pt.x = -0.70710676237;
    pt.y = 0.70710676237;
    rotate_point(&pt, asteroid->object.cos, asteroid->object.sin);
    actr_canvas2d_lineto(x + pt.x * asteroid->object.distance, y + pt.y * asteroid->object.distance);

    actr_canvas2d_lineto(x + start.x * asteroid->object.distance, y + start.y * asteroid->object.distance);

    actr_canvas2d_stroke();
}

void draw_ship(struct MyShip *ship, int thrusting, int shooting)
{
    struct ActrPointD pt;
    struct ActrPointD tip;

    double sin = actr_sin(ship->object.rotation);
    double cos = actr_cos(ship->object.rotation);
    float length = 25;
    float thrust = 10;
    float shoot = 50;

    long long x = actrState->canvasSize.w / 2;
    long long y = actrState->canvasSize.h / 2;

    actr_canvas2d_stroke_style(255, 255, 255, 100);
    actr_canvas2d_begin_path();

    actr_canvas2d_moveto(x, y);

    // start at bottom left point
    pt.x = -0.5;
    pt.y = 0.5;
    rotate_point(&pt, cos, sin);
    actr_canvas2d_lineto(x + pt.x * length, y + pt.y * length);

    // save starting point

    // line to front of ship
    pt.x = 0;
    pt.y = -0.5;
    rotate_point(&pt, cos, sin);
    tip = pt;
    actr_canvas2d_lineto(x + pt.x * length, y + pt.y * length);

    // line to bottom right
    pt.x = 0.5;
    pt.y = 0.5;
    rotate_point(&pt, cos, sin);
    actr_canvas2d_lineto(x + pt.x * length, y + pt.y * length);

    // close the path
    actr_canvas2d_lineto(x, y);
    actr_canvas2d_stroke();

    if (thrusting)
    {
        pt.x = 0;
        pt.y = 1;
        rotate_point(&pt, cos, sin);
        actr_canvas2d_stroke_style(255, 0, 0, 100);
        actr_canvas2d_begin_path();
        actr_canvas2d_moveto(x, y);
        actr_canvas2d_lineto(x + pt.x * thrust, y + pt.y * thrust);
        actr_canvas2d_stroke();
    }

    if (shooting)
    {
        pt.x = 0;
        pt.y = -0.5;
        rotate_point(&pt, cos, sin);
        actr_canvas2d_stroke_style(255, 0, 255, 100);
        actr_canvas2d_begin_path();
        actr_canvas2d_moveto(x + tip.x * length, y + tip.y * length);
        actr_canvas2d_lineto(x + pt.x * shoot * 2, y + pt.y * shoot * 2);
        actr_canvas2d_stroke();
    }
}

void drawView()
{
    long long x = state->player.object.position.x - actrState->canvasSize.w / 2;
    long long y = state->player.object.position.y - actrState->canvasSize.h / 2;

    queryView();
    actr_canvas2d_fill_style(255, 255, 255, 10);
    for (int i = 0; i < state->result->count; i++)
    {
        struct ActrQuadTreeLeaf *leaf = state->result->head[i];
        draw_asteroid((struct MyAsteroid *)leaf->item);
        // actr_canvas2d_fill_text(leaf->bounds.point.x - x, leaf->bounds.point.y - y, leaf->item);
        // actr_canvas2d_fill_rect(leaf->bounds.point.x - x, leaf->bounds.point.y - y, leaf->bounds.size.w, leaf->bounds.size.h);
    }
    state->result->count = 0;
}

struct ActrPointD fromgrid(struct ActrPoint32 point)
{
    struct ActrPointD result;
    result.x = point.x * GRID_SIZE;
    result.y = point.y * GRID_SIZE;
    return result;
}
void assmass(int w, int h, int m);
void init_area(struct ActrPoint32 grid)
{
    int index = 0;
    struct ActrPoint32 test;
    struct ActrPointD point;
    struct ActrQuadTreeBounds bounds;
    struct ActrQuadTreeLeaf *leaf;
    bounds.size.h = GRID_SIZE;
    bounds.size.w = GRID_SIZE;

    for (long long y = grid.y - 1; y < grid.y + 2; y++)
    {
        for (long long x = grid.x - 1; x < grid.x + 2; x++)
        {
            test.x = x;
            test.y = y;
            point = fromgrid(test);
            bounds.point.x = point.x;
            bounds.point.y = point.y;
            // stone 2500kg per m3
            actr_quad_tree_query(state->tree, &bounds, state->result);
            if (state->result->count == 0)
            {
                for (int i = 0; i < 9; i++)
                {
                    long long x = actr_prng() * GRID_SIZE;
                    long long y = actr_prng() * GRID_SIZE;
                    float r = actr_prng();
                    float mass = 10 + r * r * r * 1000000;
                    float rotation = actr_prng() * PI;
                    float distance = actr_sqrt(mass * MASS_SCALE);

                    if (distance < 5)
                    {
                        distance = 5;
                    }

                    int ld = distance + 1;
                    if (x < ld)
                    {
                        x += (ld - x);
                    }
                    else if (x + ld > GRID_SIZE)
                    {
                        x -= (x + ld - GRID_SIZE);
                    }
                    if (y < ld)
                    {
                        y += (ld - x);
                    }
                    else if (y + ld > GRID_SIZE)
                    {
                        y -= (y + ld - GRID_SIZE);
                    }

                    struct MyAsteroid *asteroid = init_asteroid(MyOreIce, point.x + x, point.y + y, rotation, mass, distance);

                    leaf = actr_quad_tree_leaf(point.x + x - ld, point.y + y - ld, ld * 2.0, ld * 2.0, asteroid);
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

struct MyAsteroid *init_asteroid(enum MyOre ore, double x, double y, float rotation, float mass, float distance)
{
    struct MyAsteroid *asteroid = actr_malloc(sizeof(struct MyAsteroid));
    init_object((struct MyObject *)asteroid, MyObjectTypeAsteroid, x, y, rotation, mass);
    asteroid->ore = ore;
    asteroid->object.cos = actr_cos(asteroid->object.rotation);
    asteroid->object.sin = actr_sin(asteroid->object.rotation);

    asteroid->object.distance = distance;

    return asteroid;
}

void init_object(struct MyObject *object, enum MyObjectType type, double x, double y, float rotation, float mass)
{
    object->type = type;
    object->position.x = x;
    object->position.y = y;
    object->velocity.x = 0;
    object->velocity.y = 0;
    object->rotation = rotation;
    object->mass = mass;
}


int lines_intersect(struct ActrPointF a1, struct ActrPointF a2, struct ActrPointF b1, struct ActrPointF b2)
{
    // https://stackoverflow.com/a/3746601
    struct ActrPointF intersection;
    struct ActrPointF c;
    struct ActrPointF b;
    struct ActrPointF d;
    
    intersection.x = 0;
    intersection.y = 0;

    b.x = a2.x - a1.x;
    b.y = a2.y - a1.y;

    d.x = b2.x - b1.x;
    d.y = b2.y - b1.y;

    float bDotDPerp = b.x * d.y - b.y * d.x;

    // if b dot d == 0, it means the lines are parallel so have infinite intersection points
    if (bDotDPerp == 0)
    {
        return 0;
    }

    
    c.x = b1.x - a1.x;
    c.y = b1.y - a1.y;
    float t = (c.x * d.y - c.y * d.x) / bDotDPerp;
    if (t < 0 || t > 1)
    {
        return 0;
    }

    float u = (c.x * b.y - c.y * b.x) / bDotDPerp;
    if (u < 0 || u > 1)
    {
        return 0;
    }

    // intersection = a1 + t * b;

    return 1;
}

void queryView()
{
    struct ActrQuadTreeBounds bounds;
    bounds.point.x = state->player.object.position.x - actrState->canvasSize.w / 2;
    bounds.point.y = state->player.object.position.y - actrState->canvasSize.h / 2;
    bounds.size.w = actrState->canvasSize.w;
    bounds.size.h = actrState->canvasSize.h;
    actr_quad_tree_query(state->tree, &bounds, state->result);
}

void rotate_point(struct ActrPointD *point, double cos, double sin)
{
    // double x = actr_sin(rotation);
    // double y = actr_cos(rotation);
    double x = point->x * cos - point->y * sin;
    double y = point->x * sin + point->y * cos;
    point->x = x;
    point->y = -y;
}

struct ActrPoint32 togrid(struct ActrPointD point)
{
    struct ActrPoint32 result;
    result.x = (point.x < 0 ? (point.x - GRID_SIZE) : point.x) / GRID_SIZE;
    result.y = (point.y < 0 ? (point.y - GRID_SIZE) : point.y) / GRID_SIZE;
    return result;
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

[[clang::export_name("actr_init")]]
void actr_init()
{
    state = actr_malloc(sizeof(struct MyState));
    init_object(&state->player.object, MyObjectTypeShip, 0, 0, PI, 3500);

    state->tree = actr_quad_tree_init(1, 0, 0, GRID_SIZE, 0);
    state->result = actr_vector_init(8, 8);
    struct ActrPoint32 grid;
    grid.x = 0;
    grid.y = 0;
    init_area(grid);
}

void aspect(float a);
[[clang::export_name("actr_resize")]]
void actr_resize(float w, float h)
{
    state->aspect = w / h;
    aspect(state->aspect);
}

[[clang::export_name("actr_key_down")]]
void actr_key_down(int key)
{
    state->keys[key] = 1;
}

void gotkey(double key);

[[clang::export_name("actr_key_up")]]
void actr_key_up(int key)
{
    state->keys[key] = 0;
    gotkey(key);
}

[[clang::export_name("actr_step")]]
void actr_step(double delta)
{
    double y = actr_cos(state->player.object.rotation);
    double x = actr_sin(state->player.object.rotation);
    float length = 50;
    float force = 99;
    state->player.object.rotation = wrapTAU(state->player.object.rotation);
    int shooting = 0;
    if (state->keys[32]) {
        shooting = 1;
    }
    if (state->keys[1])
    {
        state->player.object.rotation += delta * (shooting ? 1 : 5);
    }
    else if (state->keys[2])
    {
        state->player.object.rotation -= delta * (shooting ? 1 : 5);
    }
    else if (state->keys[6])
    {
        double desired = actr_atan2(-state->player.object.velocity.y, state->player.object.velocity.x) - PI / 2.0;
        double rotationError = wrapPITAU(desired - state->player.object.rotation);
        state->player.object.rotation += rotationError * delta * (shooting ? 0.2 : 0.5);
    }

    state->player.object.position.x += state->player.object.velocity.x * delta;
    state->player.object.position.y += state->player.object.velocity.y * delta;

    int wrap = 0;
    if (wrap)
    {
        if (state->player.object.position.x > actrState->canvasSize.w)
        {
            state->player.object.position.x = 0;
        }
        else if (state->player.object.position.x < 0)
        {
            state->player.object.position.x = actrState->canvasSize.w;
        }

        if (state->player.object.position.y > actrState->canvasSize.h)
        {
            state->player.object.position.y = 0;
        }
        else if (state->player.object.position.y < 0)
        {
            state->player.object.position.y = actrState->canvasSize.h;
        }
    }

    int thrusting = 0;
    if (state->keys[5])
    {
        thrusting = 1;
        state->player.object.velocity.x += x * delta * force;
        state->player.object.velocity.y += y * delta * force;
    }
    

    //*
    state->player.object.velocity.x *= 0.999999;
    state->player.object.velocity.y *= 0.999999;
    //*/

    actr_canvas2d_fill_style(0, 0, 0, 100);
    actr_canvas2d_fill_rect(-10, -10, actrState->canvasSize.w + 20, actrState->canvasSize.h + 20);

    // actr_canvas_moveto(state->player.position.x, state->player.position.y);

    // actr_canvas_lineto(state->player.position.x + x * length, state->player.position.y + y * length);
    drawView();
    draw_ship(&state->player, thrusting, shooting);

    // struct ActrFormatState *format = actr_format("pos %s");
    struct ActrPoint32 tgrid = togrid(state->player.object.position);

    if (tgrid.x != state->lastGrid.x || tgrid.y != state->lastGrid.y)
    {
        state->lastGrid = tgrid;
        init_area(tgrid);
    }

    struct ActrFormatState *format = actr_format("pos %s x %s grid %s x %s delta %s");

    // pos
    actr_format_int(format, state->player.object.position.x);
    actr_format_int(format, state->player.object.position.y);

    // grid
    actr_format_int(format, tgrid.x);
    actr_format_int(format, tgrid.y);

    actr_format_float(format, delta * 1000);

    char *text = actr_format_close(format);

    actr_canvas2d_fill_style(255, 255, 255, 100);
    actr_canvas2d_fill_text(5, actrState->canvasSize.h - 5, text);
    actr_free(text);

    text = actr_memory_report();
    actr_canvas2d_fill_text(5, actrState->canvasSize.h - 20, text);
    actr_free(text);
    struct ActrPoint64 offset;

    // offset.x = -(state->player.position.x + actrState->canvasSize.w);
    // offset.y = -(state->player.position.y + actrState->canvasSize.h);

    offset.x = (state->player.object.position.x - actrState->canvasSize.w / 2);
    offset.y = (state->player.object.position.y - actrState->canvasSize.h / 2);

    // actr_quad_tree_draw(state->tree, offset);
}