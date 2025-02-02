@echo off
clear
SET script=%1
SET server=%2


if "%script%" == "" (
  echo Example use 
  echo ./pub.bat scriptFile.c 
  exit
)

if "%script%" == "script.c" (
  SET apiKey=pmu58grtf9vbyyxsy0ozr1l78wr1otlg4vaz7o1xpcz8amfd3x
)

if "%script%" == "fetch-text.c" (
  SET apiKey=pjy2drrc8hhhvk9gx7c8ga3h3aa6l4en9rtk5sa8rbisgz0hpd
)
  
if "%script%" == "myScript" (
  SET apiKey=myApiKey
)

if "%apiKey%" == "" (
  echo apiKey not assigned for file 
)

echo Building...
clang %script% -I header --target=wasm32-unknown-unknown ^
   --optimize=3 -nostdlib -nostdlibinc -nostdinc -nostdinc++ ^
   -Wl,--no-entry -Wl,--export-all -Wl,--error-limit=0 ^
    -Wl,--allow-undefined --wasm-opt --output %script%.wasm

if %server% == local (
  SET server=https://localhost:7086/api/Wasm/Upload/
  echo SET LOCAL %server%
) else (
  SET server=https://www.d1ag0n.com/api/Wasm/Upload/
  echo SET REMOTE %server%
)
echo %server%

if %ERRORLEVEL% == 0 (
  echo Build success uploading...
  
  curl -k -F "data=@./%script%.wasm" "%server%%apiKey%%"
) else (
  echo Build failed
)
