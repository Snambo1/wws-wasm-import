
#ifndef ACTRWASM_H
#define ACTRWASM_H
#include <string.h>
#include <stdio.h>

// externs
void actr_canvas2d_fillStyle(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void actr_canvas2d_strokeStyle(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

void actr_canvas2d_beginPath();

void actr_canvas2d_ellipse(double x, double y, double radiusX, double radiusY, double rotation, double startAngle, double endAngle, char counterclockwise);

void actr_canvas2d_fill();
void actr_canvas2d_fillRect(double x, double y, double w, double h);
void actr_canvas2d_fillTextLength(double x, double y, char *text, int length);

void actr_canvas2d_stroke();
void actr_canvas2d_strokeRect(double x, double y, double w, double h);

void actr_sizeSanity(char *text, int length);
// end externs

// helpers
void actr_canvas2d_fillText(double x, double y, char *text)
{
    actr_canvas2d_fillTextLength(x, y, text, strlen(text));
}
// end helpers

struct ActrPoint
{
    int x;
    int y;
};
struct ActrState
{
    unsigned int structSize;
    struct ActrPoint canvasSize;
    struct ActrPoint pointer;
};
static struct ActrState state;

void EMSCRIPTEN_KEEPALIVE actr_doSanity()
{
    state.canvasSize.x = 0;
    state.canvasSize.y = 0;
    state.pointer.x = 0;
    state.pointer.y = 0;

    int SIZE = 64;
    char buffer[SIZE];
    snprintf(buffer, SIZE, ":%lu:%lu:%lu:%lu", sizeof(int), sizeof(double), sizeof(struct ActrState), &state);
    actr_sizeSanity(buffer, strlen(buffer));
}

#endif
