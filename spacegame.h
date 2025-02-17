#ifndef SPACEGAME_H
#define SPACEGAME_H
#include "actrwasm.h"
#include "actrui.h"
#include "actrlist.h"

#define GRID_SIZE 1024
#define MASS_SCALE 0.001

enum MyOre {
    MyOreStone,
    MyOreIce,
    MyOreIron,
    MyOreCopper,
    MyOreNickel,
    MyOreSilicon,
    MyOreEnd // used as the end if this ore is displayed in the game that is an error
};

char * OreNames[] = {
    "Stone",
    "Ice",
    "Iron",
    "Copper",
    "Nickel",
    "Silicon",
    "Error"
};

unsigned int OreColor[] = {
    0x765b4664, // stone
    0xb9e8ea32, // ice
    0xa19d9464, // iron 
    0xB8733364, // copper
    0x6C747464, // nickel
    0x80808032, // silicon
    0xff000064, // error
};

enum MyInventoryItemType {
    MyInventoryItemTypeOre
};

struct MyInventoryItem {
    enum MyInventoryItemType type;
    unsigned int quantity;
};

enum MyObjectType {
    MyObjectTypeAsteroid,
    MyObjectTypeShip,
    MyObjectTypeStation,
};

struct MyInventory {
    struct MyInventoryItem ore[MyOreEnd];
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
    int identity;
};

struct MyObjectAsteroid { 
    struct MyObject object;
    enum MyOre ore;
};

struct MyObjectShip
{
    struct MyObject object;
    
    struct MyInventory inventory;
};

enum MyMenuAction {
    MyMenuActionClose,
    MyMenuActionBack,
    MyMenuActionBuildBase,
};

struct MyMenuItem {
    enum MyMenuAction action;
    char * text;
};

struct MyMenu {
    int key;
    struct ActrVector * items;
    int position;
};

struct MyMessage {
    int age;
    char * text;
};

struct MyState
{
    struct MyObjectShip player;
    float aspect;
    char keys[256];
    struct ActrQuadTree *tree;
    struct ActrVector *result;
    struct ActrPoint32 lastGrid;
    int paused;
    int identity;
    struct MyMenu * menu;
    struct ActrList * messages;
};


// a1 is line1 start, a2 is line1 end, b1 is line2 start, b2 is line2 end

void drawAsteroid(struct MyObjectAsteroid *asteroid);
void draw_ship(struct MyObjectShip *ship, int thrusting, int shooting);
void drawView();
struct ActrPointD fromgrid(struct ActrPoint32 point);
void init_area(struct ActrPoint32 grid);
struct MyObjectAsteroid *init_asteroid(enum MyOre ore, double x, double y, float rotation, float mass, float distance);
void init_object(struct MyObject * object, enum MyObjectType type, double x, double y, float rotation, float mass);
int lines_intersect(struct ActrPointF a1, struct ActrPointF a2, struct ActrPointF b1, struct ActrPointF b2);
void queryView();
enum MyOre random_ore();
void rotate_point(struct ActrPointD *point, double cos, double sin);
struct ActrPoint32 togrid(struct ActrPointD point);
void update_ship(struct MyObjectShip *ship, double delta, float rotate, float thrust, int shooting);
double wrapN(double value, double N);
double wrapPI(double value);
double wrapPITAU(double value);
double wrapTAU(double value);


#endif
