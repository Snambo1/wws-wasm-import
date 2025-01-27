#include <string.h>
#include <emscripten.h>

// emcc -O3 -s WASM=1 -s SIDE_MODULE=2 script.c -o script.wasm
// curl -F "data=@./script.wasm" "https://localhost:7086/api/Wasm/Upload/xg2ndver6ndmwxz9rxne4v2j2la8ild43g505x01or2s0d1l1m"
// Get-FileHash -Path ".\script.wasm" -Algorithm SHA256

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


// time in ms
int main(int time) {
  // static float time = 0;
  float ftime = time;
  ftime /= 1000.0;
  

  actr_fillStyle("black");
  _actr_fillRect(0, 0, 9999, 9999);
  
  actr_fillStyle("red");
  actr_strokeStyle("white");
  _actr_fillRect(ftime, 20, 30, 40);
  _actr_strokeRect(ftime, 20, 30, 40);
  return 0;
}
