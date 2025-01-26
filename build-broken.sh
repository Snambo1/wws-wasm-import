#!/bin/bash
set -e

clang \
	--target=wasm32 \
	-nostdlib \
	-nostdinc \
	-O3 \
	-o /tmp/$2.o \
	$1


wasm-ld \
	--no-entry \
	--export-all \
	--lto-O3 \
	--allow-undefined \
	--import-memory
	/tmp/$2.o \
	-o $2
#	--initial-memory=131072 \
#	--max-memory=131072 \

#rm -rf inc.wasm.tmp*
hexdump $2 | head -n 1
#wasm-objdump -x inc.wasm