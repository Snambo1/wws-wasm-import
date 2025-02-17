#ifndef SPACEGAME_H
#define SPACEGAME_H
#include "actrwasm.h"
#define GRID_SIZE 4096
// #define GRID_SIZE 1024
#define MASS_SCALE 0.001

enum MyOre {
    MyOreStone,
    MyOreIce,
    MyOreIron,
    MyOreCopper,
    MyOreNickel,    
};

char * OreNames[] = {
    "Stone",
    "Ice",
    "Iron",
    "Copper",
    "Nickel"
};

enum MyObjectType {
    MyObjectTypeAsteroid,
    MyObjectTypeShip,
};

struct MyObject {
    enum MyObjectType type;
    struct ActrPointD position;
    struct ActrPointD velocity;
    float rotation;
    float mass;
    float sin;
    float cos;
    float distance;
};

struct MyAsteroid { 
    struct MyObject object;
    enum MyOre ore;
};

struct MyShip
{
    struct MyObject object;
};

struct MyState
{
    struct MyShip player;
    float aspect;
    char keys[256];
    struct ActrQuadTree *tree;
    struct ActrVector *result;
    struct ActrPoint32 lastGrid;
};

// a1 is line1 start, a2 is line1 end, b1 is line2 start, b2 is line2 end

void drawAsteroid(struct MyAsteroid *asteroid);
void draw_ship(struct MyShip *ship, int thrusting, int shooting);
void drawView();
struct ActrPointD fromgrid(struct ActrPoint32 point);
void init_area(struct ActrPoint32 grid);
struct MyAsteroid *init_asteroid(enum MyOre ore, double x, double y, float rotation, float mass, float distance);
void init_object(struct MyObject * object, enum MyObjectType type, double x, double y, float rotation, float mass);
int lines_intersect(struct ActrPointF a1, struct ActrPointF a2, struct ActrPointF b1, struct ActrPointF b2);
void queryView();
void rotate_point(struct ActrPointD *point, double cos, double sin);
struct ActrPoint32 togrid(struct ActrPointD point);
double wrapN(double value, double N);
double wrapPI(double value);
double wrapPITAU(double value);
double wrapTAU(double value);


#endif
