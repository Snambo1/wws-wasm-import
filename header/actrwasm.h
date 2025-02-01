
#ifndef ACTRWASM_H
#define ACTRWASM_H
// #include <stdio.h>

// all actr_canvas2d prefixed method will follow html CanvasRenderingContext2d as closly as possible
// see docs at https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D

void actr_sanity_size(int intSize, int doubleSize, int stateSize, void * state);

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

static struct ActrState actrState;

// internal use
[[clang::export_name("actr_sanity")]]
void actr_sanity()
{
    actr_sanity_size(sizeof(int), sizeof(double), sizeof(struct ActrState), &actrState);
}




#endif
