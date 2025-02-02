@echo off

clear

SET script=%1
SET server=%2

if "%script%" == "" (
  echo Example use 
  echo ./pub.bat scriptFile.c 
  exit
)

if "%script%" == "fetch-text.c" (
  SET apiKey=pjy2drrc8hhhvk9gx7c8ga3h3aa6l4en9rtk5sa8rbisgz0hpd
)

if "%script%" == "fetch-json.c" (
  SET apiKey=w3pwm3k3bzdj9jskjzo8c5axm5ffd8ygz51tzgi6q9jgp98jjj
)

if "%script%" == "myScript" (
  SET apiKey=myApiKey
)

if "%apiKey%" == "" (
  echo apiKey not assigned for file
  exit
)

if %server% == local (
  SET server=https://localhost:7086/api/Wasm/Upload/
) else (
  SET server=https://www.d1ag0n.com/api/Wasm/Upload/
)

echo Building...

clang %script% -I header --target=wasm32-unknown-unknown ^
   --optimize=3 -nostdlib -nostdlibinc -nostdinc -nostdinc++ ^
   -Wl,--no-entry -Wl,--export-all -Wl,--error-limit=0 ^
    -Wl,--allow-undefined --wasm-opt --output %script%.wasm



if %ERRORLEVEL% == 0 (
  echo Uploading...
  curl -k -F "data=@./%script%.wasm" "%server%%apiKey%%"
) else (
  echo Build failed
)
