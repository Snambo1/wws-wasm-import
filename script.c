#include <string.h>
#include <emscripten.h>
#include "actrwasm.h"

// emcc -O3 --no-entry -s WASM=1 -s ENVIRONMENT=web -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s SIDE_MODULE=2 script.c -o script.wasm
// curl -F "data=@./script.wasm" "https://localhost:7086/api/Wasm/Upload/xg2ndver6ndmwxz9rxne4v2j2la8ild43g505x01or2s0d1l1m"
// Get-FileHash -Path ".\script.wasm" -Algorithm SHA256


void actr_fillStyle(const char* value) {
  _actr_fillStyle(value, strlen(value));
}

void actr_strokeStyle(const char* value) {
  _actr_strokeStyle(value, strlen(value));
}

void actr_log(const char* buffer) {
  _actr_log(buffer, strlen(buffer));
}

static float time = 0;

EMSCRIPTEN_KEEPALIVE void step(float delta) {
  time += delta;
  // static float time = 0;
  
  

  actr_fillStyle("black");
  _actr_fillRect(0, 0, 9999, 9999);
  
  actr_fillStyle("red");
  actr_strokeStyle("white");
  _actr_fillRect(time, 20, 30, 40);
  _actr_strokeRect(time, 20, 30, 40);

}

