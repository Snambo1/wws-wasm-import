@echo off
clear
echo Building...
clang script.c -I header --target=wasm32-unknown-unknown ^
   --optimize=3 -nostdlib -nostdlibinc -nostdinc -nostdinc++ ^
   -Wl,--no-entry -Wl,--export-all -Wl,--error-limit=0 ^
    -Wl,--allow-undefined --wasm-opt --output script.wasm
if %ERRORLEVEL% == 0 (
  echo Build success uploading...
  upload.exe
) else (
  echo Build failed
)
