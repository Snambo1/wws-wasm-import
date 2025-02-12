
#ifndef ACTRWASM_H
#define ACTRWASM_H
#include "actralloc.h"

// basic header required for integration with the ActR platform
// all actr_canvas2d prefixed method will follow html CanvasRenderingContext2d as closly as possible
// see docs at https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D

extern void _actr_sanity_size(int intSize, int longSize, int floatSize, int doubleSize, int stateSize, void * state);

// end helpers
struct ActrPoint
{
    int x;
    int y;
};
struct ActrState
{
    // canvasSize will be updated automatically
    struct ActrPoint canvasSize;
    // pointer position will be updated automatically
    struct ActrPoint pointerPosition;
    // text size will be updated when actr_canvas2d_measureText is called
    struct ActrPoint textSize;
};

struct ActrState * actrState;

/// @brief internal use
[[clang::export_name("_actr_sanity")]]
void _actr_sanity()
{
    actrState = actr_malloc(sizeof(struct ActrState));
    _actr_sanity_size(sizeof(int), sizeof(long long), sizeof(float), sizeof(double), sizeof(struct ActrState), actrState);
}

/// @brief get length of null terminated string
/// @param string 
/// @return length of the string
unsigned long strlen(const char * string) {
    int result = 0;
    while (*string++) {
        result++;
    }
    return result;
}
char *actr_heap_string(char *text)
{
    int size = strlen(text);
    char *result = actr_malloc(size + 1);
    for (int i = 0; i < size; i++) {
        result[i] = text[i];
    }
    return result;
}
#endif
