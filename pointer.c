#include "actrwasm.h"


// optional this is called when the user clicks the mouse
[[clang::export_name("actr_tap")]]
void actr_tap(double x, double y)
{
  
}

// optional this is called in request animation frame
[[clang::export_name("actr_step")]]
void actr_step(double delta)
{
}