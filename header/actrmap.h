
#ifndef ACTRMAP_H
#define ACTRMAP_H
#include "actrwasm.h"

// this is all depreciated needs to be removed in favor of the actrjson.h

void actr_map_delete(int map);

long long _actr_map_get_int_length(int mapId, char *name, int nameLength);
double _actr_map_get_float_length(int mapId, char *name, int nameLength);
char *_actr_map_get_string_length(int mapId, char *name, int nameLength);

void _actr_map_set_int_length(int mapId, char *name, int nameLength, long long value);
void _actr_map_set_float_length(int mapId, char *name, int nameLength, double value);
void _actr_map_set_string_length(int mapId, char *name, int nameLength, char *value, int valueLength);

long long actr_map_get_int(int mapId, char *name)
{
    return _actr_map_get_int_length(mapId, name, strlen(name));
}
double actr_map_get_float(int mapId, char *name)
{
    return _actr_map_get_float_length(mapId, name, strlen(name));
}
char *actr_map_get_string(int mapId, char *name)
{
    return _actr_map_get_string_length(mapId, name, strlen(name));
}

void actr_map_set_int(int mapId, char *name, long long value)
{
    _actr_map_set_int_length(mapId, name, strlen(name), value);
}
void actr_map_set_float(int mapId, char *name, double value)
{
    _actr_map_set_float_length(mapId, name, strlen(name), value);
}
void actr_map_set_string(int mapId, char *name, char *value)
{
    _actr_map_set_string_length(mapId, name, strlen(name), value, strlen(value));
}

#endif
