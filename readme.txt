emcc -s STANDALONE_WASM -o script.wasm script.c
clang --target=wasm32 --no-standard-libraries -Wl,--export-all -Wl,--no-entry -o script.wasm script.c

in windows cmd
emcc script.c -Os -s WASM=1 -s SIDE_MODULE=1 -o script.wasm

