#include "actrwasm.h"

#ifndef ACTRJSON_H
#define ACTRJSON_H
        
    int _actr_json_get_int_length(int jsonId, char * path, int pathLength);
    float _actr_json_get_float_length(int jsonId, char * path, int pathLength);
    char * _actr_json_get_string_length(int jsonId, char * path, int pathLength);

    int actr_json_get_int(int jsonId, char * path) {
        return _actr_json_get_int_length(jsonId, path, strlen(path));
    }
    float actr_json_get_float(int jsonId, char * path) {
        return _actr_json_get_float_length(jsonId, path, strlen(path));
    }
    char * actr_json_get_string(int jsonId, char * path) {
        return _actr_json_get_string_length(jsonId, path, strlen(path));
    }

#endif

