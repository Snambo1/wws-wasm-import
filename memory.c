#include "actrwasm.h"
#include "actrcanvas.h"
#include "actralloc.h"

void clearScreen();
void printReport();
void printLine(char *line);
void printBar(double percent);
void draw();

int top = 0;

[[clang::export_name("actr_resize")]]
void actr_resize()
{
    draw();
}

[[clang::export_name("actr_init")]]
void actr_init()
{
    draw();
}

void draw() {
    void *memory1;
    void *memory2;
    void *memory3;

    top = 10;

    clearScreen();
    printReport();
    
    // allocate memory
    memory1 = actr_malloc(1024 * 1024 * 5);
    printReport();

    memory2 = actr_malloc(1024 * 1024 * 5);
    printReport();

    memory3 = actr_malloc(1024 * 1024 * 5);
    printReport();


    actr_free(memory1);
    printReport();
    
    actr_free(memory2);
    printReport();

    actr_free(memory3);
    printReport();
}

void printBar(double percent) {
    double width = actrState->canvasSize.x * 0.5 - 20;

    actr_canvas2d_fill_style(0, 0, 255, 100);
    actr_canvas2d_fill_rect(10, top, width * percent, 25);

    actr_canvas2d_stroke_style(0, 255, 255, 100);
    actr_canvas2d_stroke_rect(10, top, width, 25);
    
    
    top += 30;
}

void printReport() {
    char *line;
    printBar(actr_memory_usage());
    line = actr_memory_report(); // get memory report
    printLine(line);
    actr_free(line); // free allocated report string
    
}

void clearScreen()
{
    actr_canvas2d_fill_style(0, 0, 0, 100);        // set fill color to black
    actr_canvas2d_fill_rect(-10, -10, 9999, 9999); // clear the canvas
}
void printLine(char *line)
{
    actr_canvas2d_fill_style(255, 255, 255, 100);           // set fill color to white
    actr_canvas2d_fill_text(25, top - 10, line); // draw text a specified x/y position
}
