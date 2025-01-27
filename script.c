#include <string.h>
#include <emscripten.h>
#include "script.h"

// THIS
// emcc -O3 -s WASM=1 -s SIDE_MODULE=2 script.c -o script.wasm
// emcc  -O3 -s WASM=1 -s SIDE_MODULE=1 script.c  -o script.wasm
// emcc -O3 -s WASM=1 -s SIDE_MODULE=2 script.c -o script.wasm 
extern void _actr_log(const char* value, int length);

extern void _actr_fillStyle(const char* value, int length);
extern void _actr_strokeStyle(const char* value, int length);

extern void _actr_fillRect(float x, float y, float w, float h);
extern void _actr_strokeRect(float x, float y, float w, float h);


void actr_fillStyle(const char* value) {
  _actr_fillStyle(value, strlen(value));
}

void actr_strokeStyle(const char* value) {
  _actr_strokeStyle(value, strlen(value));
}

void actr_log(const char* buffer) {
  _actr_log(buffer, strlen(buffer));
}

EMSCRIPTEN_KEEPALIVE void step(double value) {

} 
// time in ms
int main() {
  // static float time = 0;
  float time = 0;
  time /= 1000.0;
  

  actr_fillStyle("black");
  _actr_fillRect(0, 0, 9999, 9999);
  
  actr_fillStyle("red");
  actr_strokeStyle("white");
  _actr_fillRect(time / 10.0, 20, 30, 40);
  _actr_strokeRect(time / 10.0, 20, 30, 40);
  return 0;
}
