
#ifndef ACTRWASM_H
#define ACTRWASM_H
#include <string.h>
#include <stdio.h>

// all actr_canvas2d prefixed method will follow html CanvasRenderingContext2d as closly as possible
// see docs at https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D
void actr_canvas2d_fillStyle(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void actr_canvas2d_strokeStyle(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

void actr_canvas2d_beginPath();

void actr_canvas2d_ellipse(double x, double y, double radiusX, double radiusY, double rotation, double startAngle, double endAngle, char counterclockwise);

void actr_canvas2d_fill();
void actr_canvas2d_fillRect(double x, double y, double w, double h);
void actr_canvas2d_fillText(double x, double y, char *text, int length);

void actr_canvas2d_measureText(char * text, int length);

void actr_canvas2d_stroke();
void actr_canvas2d_strokeRect(double x, double y, double w, double h);

void actr_sizeSanity(char *text, int length);


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
void EMSCRIPTEN_KEEPALIVE actr_doSanity()
{
    int SIZE = 64;
    char buffer[SIZE];
    snprintf(buffer, SIZE, "%lu:%lu:%lu:%lu", sizeof(int), sizeof(double), sizeof(struct ActrState), &actrState);
    actr_sizeSanity(buffer, strlen(buffer));
}

#endif
