@echo off

SET script=%1
SET local=%2

if "%script%" == "" (
  echo Example use
  echo ./pub.bat scriptFile.c
  exit
)

if "%script%" == "pieGraph.c" (
  SET apiKey=zmd86xxjg04xyghyck3ixnbreydk7idat80q1ivk554s0b8hdu
) 


if "%script%" == "myScript" (
  SET apiKey=myApiKey
)

if "%apiKey%" == "" (
  echo apiKey not assigned for %script%
  exit
)

SET server=https://www.d1ag0n.com/api/Wasm/Upload/

REM echo Building...
REM -fno-inline -nostdlib -nostdlibinc -nostdinc -nostdinc++  -fno-builtin
emcc -I header  -O3 --no-entry -s WASM=1 -s ENVIRONMENT=web -s ERROR_ON_UNDEFINED_SYMBOLS=0 -Wl,--export=__heap_base %script% -o %script%.wasm
curl --fail -F "data=@./%script%.wasm" "%server%%apiKey%"


if %ERRORLEVEL% == 0 (
  echo.
  echo %script%.wasm uploaded to %server%
  rm %script%.wasm
) else (
  echo %script%.wasm upload failed
)
