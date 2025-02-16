
#ifndef ACTRWASM_H
#define ACTRWASM_H
#include "actralloc.h"
#define PI 3.14
#define TAU 6.28

// basic header required for integration with the ActR platform
// all actr_canvas2d prefixed method will follow html CanvasRenderingContext2d as closly as possible
// see docs at https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D

extern double actr_sin(double value);
extern double actr_cos(double value);
extern double actr_atan2(double y, double x);
extern void _actr_sanity_size(int intSize, int longSize, int floatSize, int doubleSize, int stateSize, void * state);
extern int actr_authenticated();
// end helpers
struct ActrPoint
{
    long long x; // 8
    long long y; // 8
};

struct ActrSize
{
    long long w; // 4
    long long h; // 4
};

struct ActrSizeF
{
    float w; // 4
    float h; // 4
};

struct ActrState
{
    // canvasSize will be updated automatically
    struct ActrSizeF canvasSize; // 8
    // pointer position will be updated automatically
    struct ActrPoint pointerPosition; // 16
    // text size will be updated when actr_canvas2d_measureText is called
    struct ActrPoint textSize; // 16
    int debug; // 4
};

struct ActrState * actrState;

unsigned int actr_pack_bytes(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    return (r << 24) | (g << 16) | (b << 8) | a;
}
void actr_unpack_bytes(unsigned int value, unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *a)
{
    *r = (value >> 24) & 0xFF;
    *g = (value >> 16) & 0xFF;
    *b = (value >> 8) & 0xFF;
    *a = value & 0xFF;
}
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

char *substr(char * text, int start, int length) {
    if (length == 0) {
        length = strlen(text) - start;
    }
    char * newText = actr_malloc(length + 1);

    for (int i = 0; i < length; i++) {
        newText[i] = text[start + i]; 
    }
    return newText;
}
void actr_heap_string(char ** target, char *text)
{
    if (*target != 0)
    {
        actr_free(*target);
    }
    int size = strlen(text);
    char *result = actr_malloc(size + 1);
    for (int i = 0; i < size; i++) {
        result[i] = text[i];
    }
    *target = result;
}
#endif
