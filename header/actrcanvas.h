#ifndef ACTRCANVAS_H
#define ACTRCANVAS_H

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
#endif