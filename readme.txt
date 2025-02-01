recommended compilation method
clang script.c --target=wasm32-unknown-unknown --optimize=3 -nostdlib -nostdlibinc -nostdinc -nostdinc++ -Wl,--no-entry -Wl,--allow-undefined --wasm-opt --output script.wasm

Create a new program entry on the site @ https://www.d1ag0n.com/ng/program/manager
Click on the new program entry
Click 'Generate API Key'

example key abcdefghijklmnopqrstuvwxyz

POST the WASM using the API key to https://www.d1ag0n.com/api/Wasm/Upload/abcdefghijklmnopqrstuvwxyz

or configure the upload cli by running upload.exe