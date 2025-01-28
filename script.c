
#include <emscripten.h>
#include "actrwasm.h"
// emcc -O3 --no-entry -s WASM=1 -s ENVIRONMENT=web -s ERROR_ON_UNDEFINED_SYMBOLS=1 -s SIDE_MODULE=2 script.c -o script.wasm
// emcc -O3 --no-entry -s WASM=1 -s ENVIRONMENT=web -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s SIDE_MODULE=2 script.c -o script.wasm
// curl -F "data=@./script.wasm" "https://localhost:7086/api/Wasm/Upload/xg2ndver6ndmwxz9rxne4v2j2la8ild43g505x01or2s0d1l1m"
// Get-FileHash -Path ".\script.wasm" -Algorithm SHA256
// or use upload.exe to upload
// or get cli @ 





static float time = 0;

void EMSCRIPTEN_KEEPALIVE step(float delta) {
  time += delta * 0.001;
  // static float time = 0;
  
  

  actr_canvas2d_fillStyle(0, 0, 0, 100);
  actr_canvas2d_fillRect(0, 0, 9999, 9999);
  
  actr_canvas2d_fillStyle(255, 0, 0, 100);
  actr_canvas2d_strokeStyle(255, 255, 255, 100);
  actr_canvas2d_fillRect(time, 20, 30, 40);
  actr_canvas2d_strokeRect(time, 20, 30, 40);

  actr_canvas2d_fillStyle(0, 255, 0, 100);
  actr_canvas2d_fillText(time, 80, "Hello World!");

}

