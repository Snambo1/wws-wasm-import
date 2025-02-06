#ifndef ACTRCANVAS_H
#define ACTRCANVAS_H

extern void actr_canvas2d_fill_style(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
extern void actr_canvas2d_stroke_style(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

extern void actr_canvas2d_begin_path();

extern void actr_canvas2d_ellipse(float x, float y, float radiusX, float radiusY, float rotation, float startAngle, float endAngle, char counterclockwise);

extern void actr_canvas2d_fill();
extern void actr_canvas2d_fill_rect(float x, float y, float w, float h);
extern void _actr_canvas2d_fill_text_length(float x, float y, char *text, int length);

extern void actr_canvas2d_measure_text(char * text, int length);

extern void actr_canvas2d_stroke();
extern void actr_canvas2d_stroke_rect(float x, float y, float w, float h);

void actr_canvas2d_fill_text(float x, float y, char *text) {
    _actr_canvas2d_fill_text_length(x, y, text, strlen(text));
}

#endif