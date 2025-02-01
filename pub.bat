@echo off
clear
clang script.c -I header --target=wasm32-unknown-unknown --optimize=3 -nostdlib -nostdlibinc -nostdinc -nostdinc++ -Wl,--no-entry -Wl,--allow-undefined --wasm-opt --output script.wasm
if %ERRORLEVEL% == 0 (
  upload.exe
) else (
  echo Build failed
)
