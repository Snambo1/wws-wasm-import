#ifndef ACTRCANVAS_H
#define ACTRCANVAS_H

extern void actr_canvas2d_fill_style(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
extern void actr_canvas2d_stroke_style(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

extern void actr_canvas2d_begin_path();

extern void actr_canvas2d_ellipse(double x, double y, double radiusX, double radiusY, double rotation, double startAngle, double endAngle, char counterclockwise);

extern void actr_canvas2d_fill();
extern void actr_canvas2d_fill_rect(double x, double y, double w, double h);
extern void _actr_canvas2d_fill_text_length(double x, double y, char *text, int length);

extern void actr_canvas2d_measure_text(char * text, int length);

extern void actr_canvas2d_stroke();
extern void actr_canvas2d_stroke_rect(double x, double y, double w, double h);

void actr_canvas2d_fill_text(double x, double y, char *text) {
    _actr_canvas2d_fill_text_length(x, y, text, strlen(text));
}

#endif