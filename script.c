#include "actrwasm.h"
#include "actrmap.h"
#include "actrcanvas.h"
#include "actrlog.h"
#include "actrtime.h"
#include "actralloc.h"

// recommended compilation method
// clang script.c --target=wasm32-unknown-unknown --optimize=3 -nostdlib -nostdlibinc -nostdinc -nostdinc++ -Wl,--no-entry -Wl,--allow-undefined --wasm-opt --output script.wasm

// untested emscripten method
// emcc -I header  -O3 --no-entry -s WASM=1 -s ENVIRONMENT=web -s ERROR_ON_UNDEFINED_SYMBOLS=0 -Wl,--export=__heap_base space-game.c -o C:\actrcontent\userwasm\10009.wasm

// emcc -I header  -O3 --no-entry -s WASM=1 -s ENVIRONMENT=web -s ERROR_ON_UNDEFINED_SYMBOLS=0  space-game.c -o C:\actrcontent\userwasm\10009.wasm
// previous attempts
// clang script.c --target=wasm32-unknown-unknown --optimize=3 -nostdlib -nostdlibinc -nostdinc -nostdinc++ -Wl,--export-all -Wl,--no-entry -Wl,--allow-undefined --output script.wasm
// clang script.c --target=wasm32-unknown-unknown --optimize=3 -nostdlib -Wl,--export-all -Wl,--no-entry -Wl,--allow-undefined --output script.wasm
// ENVIRONMENT is probably only related to the JS that is generated
// emcc -O3 --no-entry -s WASM=1 -s ENVIRONMENT=worker -s ERROR_ON_UNDEFINED_SYMBOLS=0 script.c -o script.wasm
// using SIDE_MODULE may cause some standard function to be culled from the assembly, like strlen and functions from math.h
// emcc -O3 --no-entry -s WASM=1 -s ENVIRONMENT=web -s ERROR_ON_UNDEFINED_SYMBOLS=0 script.c -o script.wasm
// vvv NO vvv
// emcc -O3 --no-entry -s WASM=1 -s ENVIRONMENT=web -s ERROR_ON_UNDEFINED_SYMBOLS=1 -s SIDE_MODULE=2 script.c -o script.wasm
// emcc -O3 --no-entry -s WASM=1 -s ENVIRONMENT=web -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s SIDE_MODULE=2 script.c -o script.wasm
// curl -F "data=@./script.wasm" "https://localhost:7086/api/Wasm/Upload/xg2ndver6ndmwxz9rxne4v2j2la8ild43g505x01or2s0d1l1m"
// Get-FileHash -Path ".\script.wasm" -Algorithm SHA256
// or use upload.exe to upload
// or get cli @

double time = 0;
int click = 0;

const int MAP = 99;



// optional, called first
[[clang::export_name("actr_init")]]
void actr_init()
{
  actr_map_set_int(MAP, "click", 0);
}

[[clang::export_name("actr_async_result")]]
void actr_async_result(int handle)
{
  
}


// optional this is called when the user clicks the mouse
[[clang::export_name("actr_tap")]]
void actr_tap(double x, double y)
{
  click++;
  actr_malloc(1);
}

// optional this is called in request animation frame
[[clang::export_name("actr_step")]]
void actr_step(double delta)
{
  actr_map_set_int(MAP, "click", actr_map_get_int(MAP, "click") + 1);

  time += delta * 0.001;

  double x = 10; // fmod(time, 100.0);
  // double x = 22;
  // put some info into a string
  int SIZE = 128;
  char buffer[SIZE];
  // snprintf(buffer, SIZE, "whatever click count: %i x:%i y:%i w:%i h:%i", click, actrState.pointerPosition.x, actrState.pointerPosition.y, actrState.canvasSize.x, actrState.canvasSize.y);
  // measure the string, result will be placed into actrState.textSize
  // actr_canvas2d_measureText(buffer, 10);

  struct ActrPoint p = actrState->pointerPosition;

  // clear background to black
  actr_canvas2d_fill_style(0, 0, 0, 100);
  actr_canvas2d_fill_rect(0, 0, 9999, 9999);

  // set fill style to red @ 100%
  actr_canvas2d_fill_style(255, 0, 0, 100);

  // set fill style to white @ 100%
  actr_canvas2d_stroke_style(255, 255, 255, 100);

  // set fill style to cyan at 80% transparency
  actr_canvas2d_fill_style(0, 200, 200, 80);
  

  // draw the text at the mouse position
  char *value = actr_memory_report();
  actr_canvas2d_fill_text(5, 10, value);
  actr_free(value);

  value = actr_time_string();
  actr_canvas2d_fill_text(5, 20, value);
  actr_free(value);

  int radius = 20;

  // draw a moving ellipse near the mouse
  actr_canvas2d_fill_style(255, 0, 0, 50);
  actr_canvas2d_begin_path();
  // actr_canvas2d_ellipse(p.x - radius / 2, p.y - radius / 2, radius, radius, time, 0, fmod(time, 6.28), 0);
  actr_canvas2d_fill();

  actr_canvas2d_begin_path();
  // actr_canvas2d_ellipse(p.x - radius / 2, p.y - radius / 2, radius, radius, time, 0, fmod(time, 6.28), 0);
  actr_canvas2d_stroke();
}
