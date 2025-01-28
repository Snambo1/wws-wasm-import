
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

// optional called first
void EMSCRIPTEN_KEEPALIVE init()
{

}


// optional this is called when the user clicks the mouse
void EMSCRIPTEN_KEEPALIVE tap(double x, double y)
{
  click++;
}

// optional this is called roughly at 60fps
void EMSCRIPTEN_KEEPALIVE step(double delta)
{

  time += delta * 0.001;

  double x = fmod(time, 100.0);
  
  // put some info into a string
  int SIZE = 128;
  char buffer[SIZE];
  snprintf(buffer, SIZE, "whatever click count: %i x:%i y:%i w:%i h:%i", click, actrState.pointerPosition.x, actrState.pointerPosition.y, actrState.canvasSize.x, actrState.canvasSize.y);
  // measure the string, result will be placed into actrState.textSize
  actr_canvas2d_measureText(buffer, strlen(buffer));
  
  struct ActrPoint p = actrState.pointerPosition;

  // clear background to black
  actr_canvas2d_fillStyle(0, 0, 0, 100);
  actr_canvas2d_fillRect(0, 0, 9999, 9999);

  // set fill style to red @ 100%
  actr_canvas2d_fillStyle(255, 0, 0, 100);
  
  // set fill style to white @ 100%
  actr_canvas2d_strokeStyle(255, 255, 255, 100);

  // draw an outline around the text
  actr_canvas2d_strokeRect(p.x, p.y, actrState.textSize.x + 4, actrState.textSize.y + 6);

  // set fill style to cyan at 80% transparency
  actr_canvas2d_fillStyle(0, 200, 200, 80);

  // draw the text at the mouse position
  actr_canvas2d_fillText(p.x, p.y + actrState.textSize.y, buffer, strlen(buffer));

  int radius = 20;

  // draw a moving ellipse near the mouse
  actr_canvas2d_fillStyle(255, 0, 0, 50);
  actr_canvas2d_beginPath();
  actr_canvas2d_ellipse(p.x - radius / 2, p.y - radius / 2, radius, radius, time, 0, fmod(time, 6.28), 0);
  actr_canvas2d_fill();

  actr_canvas2d_beginPath();
  actr_canvas2d_ellipse(p.x - radius / 2, p.y - radius / 2, radius, radius, time, 0, fmod(time, 6.28), 0);
  actr_canvas2d_stroke();

}
