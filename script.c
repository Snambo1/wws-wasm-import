
#include <emscripten.h>
#include <math.h>
#include "actrwasm.h"

// using SIDE_MODULE may cause some standard function to be culled from the assembly, like strlen and functions from math.h
// emcc -O3 --no-entry -s WASM=1 -s ENVIRONMENT=web -s ERROR_ON_UNDEFINED_SYMBOLS=0 script.c -o script.wasm
// vvv NO vvv
// emcc -O3 --no-entry -s WASM=1 -s ENVIRONMENT=web -s ERROR_ON_UNDEFINED_SYMBOLS=1 -s SIDE_MODULE=2 script.c -o script.wasm
// emcc -O3 --no-entry -s WASM=1 -s ENVIRONMENT=web -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s SIDE_MODULE=2 script.c -o script.wasm
// curl -F "data=@./script.wasm" "https://localhost:7086/api/Wasm/Upload/xg2ndver6ndmwxz9rxne4v2j2la8ild43g505x01or2s0d1l1m"
// Get-FileHash -Path ".\script.wasm" -Algorithm SHA256
// or use upload.exe to upload
// or get cli @

static double time = 0;
static int click = 0;
void EMSCRIPTEN_KEEPALIVE init()
{

}

void EMSCRIPTEN_KEEPALIVE tap(double x, double y)
{
  click++;
}

void EMSCRIPTEN_KEEPALIVE step(double delta)
{

  time += delta * 0.001;

  double x = fmod(time, 100.0);
  int SIZE = 128;
  char buffer[SIZE];
  snprintf(buffer, SIZE, "click count: %i x:%i y:%i w:%i h:%i", click, actrState.pointerPosition.x, actrState.pointerPosition.y, actrState.canvasSize.x, actrState.canvasSize.y);
    

  actr_canvas2d_fillStyle(0, 0, 0, 10);
  actr_canvas2d_fillRect(0, 0, 9999, 9999);

  actr_canvas2d_fillStyle(255, 0, 0, 50);
  actr_canvas2d_strokeStyle(255, 255, 255, 100);

  //  actr_canvas2d_fillRect(x, 10, 60, 15);
  actr_canvas2d_strokeRect(x, 10, 60, 14);

  actr_canvas2d_fillStyle(0, 200, 200, 100);
  actr_canvas2d_fillText(x + 5, 20, buffer);

  actr_canvas2d_fillStyle(255, 0, 0, 50);
  actr_canvas2d_beginPath();
  actr_canvas2d_ellipse(50, 100, 40, 40, time, 0, fmod(time, 6.28), 0);
  actr_canvas2d_fill();

}
