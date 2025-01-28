
#ifndef ACTRWASM_H
#define ACTRWASM_H
#include <string.h>
void actr_canvas2d_fillStyle(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void actr_canvas2d_strokeStyle(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void actr_canvas2d_fillRect(float x, float y, float w, float h);
void actr_canvas2d_strokeRect(float x, float y, float w, float h);
void actr_canvas2d_fillTextLength(float x, float y, char * text, int length);

void actr_canvas2d_fillText(float x, float y, char *text) {
    actr_canvas2d_fillTextLength(x, y, text, strlen(text));
}


#endif
