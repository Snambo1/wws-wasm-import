#include "actrwasm.h"
#include "actrquadtree.h"
#include "actrcanvas.h"

struct ActrQuadTree *tree;
struct ActrVector *list;
char *name;
[[clang::export_name("actr_init")]]
void actr_init() {
    list = actr_malloc(sizeof(struct ActrVector));
    list->length = 0;
    list->pointer = 0;
    // tree = actr_quad_tree_init();
    
    //char *name2 = actr_memory_report();
    //char *name3 = actr_memory_report();
    //char *name4 = actr_memory_report();
    name = actr_memory_report();
    actr_vector_add(list, (int)name);
    name = actr_memory_report();
    actr_vector_add(list, (int)name);
    //actr_vector_add(list, name2);
    //actr_vector_add(list, name3);
    //actr_vector_add(list, name4);
}

int top = 0;
void printLine(int line) {
    top += 15;
    actr_canvas2d_fill_text(10, top, (char*)line);
}
[[clang::export_name("actr_step")]]
void actr_step() {
    top = 0;
    actr_canvas2d_fill_style(0,0,0,100);
    actr_canvas2d_fill_rect(-10,-10,9999,9999);
    actr_canvas2d_fill_style(255,255,255,100);
    //log_int((char*)*(int*)list->head);
    log_int((int)list->head);
    log_int(*(int*)list->head);
    log_int(*(int*)(list->head + sizeof(int)));
    printLine(*(int*)list->head);
    printLine(*(int*)(list->head + sizeof(void*)));
    // printLine(list->head + sizeof(void*));
    
}



