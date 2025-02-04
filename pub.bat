@echo off

clear

SET script=%1
SET local=%2

if "%script%" == "" (
  echo Example use
  echo ./pub.bat scriptFile.c
  exit
)

if "%script%" == "fetch-text.c" (
  if "%local%" == "local" (
    SET apiKey=1y6dl9ncmx2egdp5wdemuttka22ss10h76lcbscxlqqt2v5rrb
  ) else (
    SET apiKey=0
  )
)

if "%script%" == "fetch-json.c" (
  if "%local%" == "local" (
    SET apiKey=ndkrj0cgsqxls7fktheaot4p13i6n0l21qp1giajqsuc8crztp
  ) else (
    SET apiKey=0
  )
)

if "%script%" == "script.c" (
  if "%local%" == "local" (
    SET apiKey=yp4ojgaouw4ijvluhwwdpujavxoyw9c4kna1hq0pisgyu4sy2s
  ) else (
    SET apiKey=0
  )
)

if "%script%" == "hello-world.c" (
  if "%local%" == "local" (
    SET apiKey=uxgksrp1uz4kt6zb6an5osgcy9xm8qj0y103rn5g4b4drytovq
  ) else (
    SET apiKey=0
  )
) else if "%script%" == "time.c" (
  if "%local%" == "local" (
    SET apiKey=wyg216v0h75ti71mrsf3ud7qirna9ohyblou55sdqu6d5svq7t
  ) else (
    SET apiKey=0
  )
) else if "%script%" == "memory.c" (
  if "%local%" == "local" (
    SET apiKey=m0m0jmeppsl0w547m7yeoto0kqen2c1wcwg22gq1iiqv29i2ht
  ) else (
    SET apiKey=0
  )
)


if "%script%" == "myScript" (
  SET apiKey=myApiKey
)

if "%apiKey%" == "" (
  echo apiKey not assigned for file
  exit
)

if "%local%" == "local" (
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
  if "%local%" == "local" (
    curl -k --fail -F "data=@./%script%.wasm" "%server%%apiKey%%"
    ) else (
    curl --fail -F "data=@./%script%.wasm" "%server%%apiKey%%"
  )
  ) else (
  echo Build failed
  exit
)

if %ERRORLEVEL% == 0 (
  echo.
  echo %script%.wasm uploaded
  rm %script%.wasm
) else (
  echo %script%.wasm upload failed
)
