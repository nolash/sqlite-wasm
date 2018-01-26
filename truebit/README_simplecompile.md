# EMSCRIPTEN SETUP

https://github.com/juj/emsdk @ 2324e5d8
* `LLVM_CMAKE_ARGS="-DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD=WebAssembly" emsdk install sdk-tag-1.37.28-64bit` (be patient!)

To activate correct paths for emscripten and dependencies use:

* `./emsdk activate sdk-tag-1.37.28-64bit`
* `./emsdk_env.sh`

# TRUEBIT SETUP

https://github.com/TrueBitFoundation/ocaml-offchain @ 9e3fc4ff
* `cd interpreter && make` -> will build executable *wasm*

https://github.com/TrueBitFoundation/emscripten-module-wrapper @ 46948d8d

* In `/prepare.js` paths are hardcoded. Edit the lines at the top starting with `var dir = ` and `var wasm = ` to reflect own setup. The latter is the *wasm* executable above (NOT the submodule)

https://github.com/TrueBitFoundation/coindrop @ 6b92edaa

* `touch input.bin`
* `touch output.bin`
* `EMCC_WASM_BACKEND=1 emcc -s WASM=1 simple.c -o simple.js` (this step first downloaded and compiled additional code)
-> will produce:
```
simple.js
simple.wast.mappedGlobals
simple.wasm
simple.wast
```
* `/path/to/emscripten-module-wrapper/prepare.js simple.js --file input.bin --file output.bin`
-> produces a dir with content as such:
```
-rw-r--r-- 1 lash lash    185 Jan 18 18:46 globals.json
-rw-r--r-- 1 lash lash  42436 Jan 18 18:46 globals.wasm
-rw-r--r-- 1 lash lash      0 Jan 18 18:46 input.data
-rw-r--r-- 1 lash lash  42335 Jan 18 18:46 merge.wasm
-rw-r--r-- 1 lash lash      0 Jan 18 18:46 output.data
-rw-r--r-- 1 lash lash 238222 Jan 18 18:46 prepared.js
-rw-r--r-- 1 lash lash      0 Jan 18 18:46 record.bin
-rw-r--r-- 1 lash lash  32795 Jan 18 18:46 simple.wasm
-rw-r--r-- 1 lash lash  33180 Jan 18 18:46 underscore.wasm
```
