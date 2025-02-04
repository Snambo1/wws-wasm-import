
#ifndef ACTRWASM_H
#define ACTRWASM_H
#include "actralloc.h"

// basic header required for integration with the ActR platform
// all actr_canvas2d prefixed method will follow html CanvasRenderingContext2d as closly as possible
// see docs at https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D

extern void _actr_sanity_size(int intSize, int doubleSize, int stateSize, void * state);

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
    _actr_sanity_size(sizeof(int), sizeof(double), sizeof(struct ActrState), actrState);
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

#endif
